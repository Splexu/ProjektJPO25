#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QList>

struct Station {
    int id;
    QString name;
    double latitude;
    double longitude;
};

struct Sensor {
    int id;
    QString paramName;
};

struct Measurement {
    QString paramName;
    double value;
    QDateTime dateTime;
};

/**
 * @class AirQualityManager
 * @brief Zarządza danymi jakości powietrza pobieranymi z API.
 */
class AirQualityManager : public QObject {
    Q_OBJECT

public:
    explicit AirQualityManager(QObject *parent = nullptr);

    /// @brief Pobiera listę wszystkich stacji.
    void fetchStations();

    /// @brief Pobiera listę sensorów dla danej stacji.
    void fetchSensors(int stationId);

    /// @brief Pobiera dane pomiarowe dla danego sensora.
    void fetchSensorData(int sensorId);

    /// @brief Pobiera współrzędne geograficzne dla podanego adresu.
    void fetchCoordinates(const QString &address);

signals:
    void stationsFetched(const QList<Station> &stations);
    void sensorsFetched(const QList<Sensor> &sensors);
    void measurementsFetched(const QList<Measurement> &measurements);
    void coordinatesFetched(double latitude, double longitude);
    void errorOccurred(const QString &error);

private:
    QNetworkAccessManager *networkManager;
};
