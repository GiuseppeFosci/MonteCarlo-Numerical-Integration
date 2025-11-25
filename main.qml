import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts

ApplicationWindow {
    id: root
    width: 1000
    height: 750 // Leggermente più alto per far respirare il layout
    visible: true
    title: "Monte Carlo Integrator - Modern UI"

    // --- IMPOSTAZIONE FONT MODERNO ---
    // Proviamo "Segoe UI" (Windows), poi "Ubuntu" (Linux), infine generico.
    font.family: "Segoe UI, Ubuntu, sans-serif"
    font.pointSize: 11

    property double integralResult: 0.0
    property double integralError: 0.0
    property string errorMessage: ""

    Connections {
        target: integrationController
        function onCalculationDone(estimate, error) {
            integralResult = estimate
            integralError = error
            errorMessage = ""
        }
        function onCalculationError(message) {
            errorMessage = message
        }
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 20 // Margini più ampi per un look più pulito
        columns: 1
        rowSpacing: 15

        // --- INPUT ---
        GroupBox {
            title: "Parametri di Configurazione"
            Layout.fillWidth: true
            Layout.preferredHeight: 140
            
            // Stile del titolo del GroupBox
            font.bold: true 
            font.pointSize: 12

            GridLayout {
                columns: 4
                width: parent.width
                columnSpacing: 20
                rowSpacing: 10
                
                // Reset del font per i controlli interni (non vogliamo tutto bold)
                // Usiamo un componente Label custom per comodità o proprietà inline
                
                Label { text: "Funzione f(x):"; font.bold: false; font.pointSize: 11 }
                TextField { 
                    id: formulaInput; 
                    text: "sin(x)"; 
                    Layout.fillWidth: true 
                    placeholderText: "Es: x*x, cos(x), exp(x)"
                    font.pointSize: 11
                }

                Label { text: "Campioni:"; font.bold: false; font.pointSize: 11 }
                TextField { 
                    id: samplesInput; 
                    text: "100000"; 
                    font.pointSize: 11
                    validator: IntValidator { bottom: 100 } 
                }

                Label { text: "Min (a):"; font.bold: false; font.pointSize: 11 }
                TextField { 
                    id: lowerBoundInput; 
                    text: "0"; 
                    font.pointSize: 11
                    validator: DoubleValidator { locale: "C" } 
                }

                Label { text: "Max (b):"; font.bold: false; font.pointSize: 11 }
                TextField { 
                    id: upperBoundInput; 
                    text: "3.14159"; 
                    font.pointSize: 11
                    validator: DoubleValidator { locale: "C" } 
                }
                
                CheckBox { 
                    id: useStratified; 
                    text: "Usa Stratified Monte Carlo"; 
                    checked: true; 
                    Layout.columnSpan: 2 
                    font.pointSize: 11
                    font.bold: false
                }
                
                Button {
                    text: "CALCOLA E GRAFICA"
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                    highlighted: true
                    
                    // Stile del bottone
                    font.bold: true
                    font.pointSize: 11
                    
                    onClicked: {
                        var min = parseFloat(lowerBoundInput.text);
                        var max = parseFloat(upperBoundInput.text);
                        var numSamples = parseInt(samplesInput.text);

                        console.log("--- INIZIO CALCOLO ---");
                        integrationController.calculate(formulaInput.text, min, max, numSamples, useStratified.checked);
                        var rawPoints = integrationController.generateFunctionPoints(formulaInput.text, min, max, 200);

                        if (rawPoints.length === 0) return;

                        seriesUpper.clear();
                        seriesLower.clear();

                        var minY = 0; var maxY = 0;

                        for(var i = 0; i < rawPoints.length; i++) {
                            var p = rawPoints[i];
                            seriesUpper.append(p.x, p.y);
                            seriesLower.append(p.x, 0);

                            if (i === 0) { minY = p.y; maxY = p.y; } 
                            else {
                                if (p.y > maxY) maxY = p.y;
                                if (p.y < minY) minY = p.y;
                            }
                        }

                        var margin = (maxY - minY) * 0.1;
                        if (margin === 0) margin = 0.5;

                        axisX.min = min; axisX.max = max;
                        axisY.min = minY - margin; axisY.max = maxY + margin;
                        
                        console.log("Grafico aggiornato.");
                    }
                }
            }
        }

        // --- GRAFICO ---
        ChartView {
            id: chart
            Layout.fillWidth: true
            Layout.fillHeight: true
            antialiasing: true
            theme: ChartView.ChartThemeLight
            
            // --- PERSONALIZZAZIONE FONT GRAFICO ---
            title: "Area Sottesa (Integrale)"
            titleFont: Qt.font({family: root.font.family, pointSize: 16, bold: true})
            
            // Colore di sfondo bianco pulito
            backgroundColor: "#ffffff"

            ValueAxis { 
                id: axisX; 
                titleText: "x" 
                labelsFont: Qt.font({family: root.font.family, pointSize: 10})
                titleFont: Qt.font({family: root.font.family, pointSize: 11, bold: true})
            }
            ValueAxis { 
                id: axisY; 
                titleText: "y" 
                labelsFont: Qt.font({family: root.font.family, pointSize: 10})
                titleFont: Qt.font({family: root.font.family, pointSize: 11, bold: true})
            }

            AreaSeries {
                name: "Area Integrale"
                axisX: axisX
                axisY: axisY
                color: "#80209fdf" // Un blu leggermente più saturo e trasparente
                borderColor: "#0078d7" // Blu stile Windows 11
                borderWidth: 2

                upperSeries: LineSeries { id: seriesUpper }
                lowerSeries: LineSeries { id: seriesLower }
            }
        }

        // --- RISULTATI ---
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            color: "#f3f3f3" // Grigio molto chiaro, moderno
            radius: 8 // Bordi arrotondati
            border.color: "#e0e0e0"

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 5
                
                RowLayout {
                    spacing: 10
                    Label { 
                        text: "Risultato Integrale:" 
                        font.pointSize: 12
                        color: "#555555"
                    }
                    Label { 
                        text: integralResult.toFixed(6)
                        font.pointSize: 18 // Numero grande
                        font.bold: true
                        color: "#0078d7" // Colore Accent (Blu)
                    }
                }

                RowLayout {
                    spacing: 10
                    Label { 
                        text: "Errore Stimato: ±" + integralError.toFixed(6)
                        color: "#666666"
                        font.pointSize: 11
                    }
                    Label {
                        text: errorMessage
                        color: "#d13438" // Rosso moderno
                        visible: errorMessage !== ""
                        font.bold: true
                    }
                }
            }
        }
    }
}