#pragma once
#include <QMap>
#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include "AirQualityManager.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QComboBox>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    double calculateDistance(double lat1, double lon1, double lat2, double lon2);
    void analyzeMeasurements(const QList<Measurement> &measurements);

private slots:
    void onStationsFetched(const QList<Station> &stations);
    void onStationClicked(QListWidgetItem *item);
    void onSensorsFetched(const QList<Sensor> &sensors);
    void onSensorClicked(QListWidgetItem *item);
    void onMeasurementsFetched(const QList<Measurement> &measurements);
    void onSearchTextChanged(const QString &text);
    void onFindStationsInRadiusClicked();
    void onCoordinatesFetched(double latitude, double longitude); // Nowy slot
    void onSaveDataClicked();
    void onErrorOccurred(const QString &error);
    void onPeriodChanged(const QString &period);

private:
    void saveStationsToJson(const QList<Station> &stations);
    void saveSensorsToJson(const QList<Sensor> &sensors);
    void saveMeasurementsToJson(const QList<Measurement> &measurements);
    QList<Station> loadStationsFromJson();
    QList<Sensor> loadSensorsFromJson();
    QList<Measurement> loadMeasurementsFromJson();
    void updateChart(const QList<Measurement> &measurements);

    QLineEdit *searchLineEdit;
    QLineEdit *addressLineEdit;
    QLineEdit *radiusLineEdit;
    QPushButton *findStationsButton;
    QPushButton *saveDataButton;
    QListWidget *stationListWidget;
    QListWidget *sensorListWidget;
    QListWidget *measurementListWidget;
    QTextEdit *analysisTextEdit;
    QComboBox *periodComboBox;
    QChartView *chartView;
    QLineSeries *series;
    AirQualityManager *aqManager;

    QList<Station> stations;
    QList<Station> filteredStations;
    QList<Sensor> sensors;
    QList<Measurement> measurements;
};
