#include "AirQualityManager.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>

AirQualityManager::AirQualityManager(QObject *parent) : QObject(parent) {
    networkManager = new QNetworkAccessManager(this);
}

void AirQualityManager::fetchStations() {
    QUrl url("https://api.gios.gov.pl/pjp-api/rest/station/findAll");
    QNetworkReply *reply = networkManager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QList<Station> stations;
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray array = doc.array();
            for (const QJsonValue &value : array) {
                QJsonObject obj = value.toObject();
                Station s;
                s.id = obj["id"].toInt();
                s.name = obj["stationName"].toString();
                s.latitude = obj["gegrLat"].toString().toDouble();
                s.longitude = obj["gegrLon"].toString().toDouble();
                stations.append(s);
            }
            emit stationsFetched(stations);
        } else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    });
}

void AirQualityManager::fetchSensors(int stationId) {
    QUrl url(QString("https://api.gios.gov.pl/pjp-api/rest/station/sensors/%1").arg(stationId));
    QNetworkReply *reply = networkManager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QList<Sensor> sensors;
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray array = doc.array();
            for (const QJsonValue &value : array) {
                QJsonObject obj = value.toObject();
                Sensor s;
                s.id = obj["id"].toInt();
                s.paramName = obj["param"].toObject()["paramName"].toString();
                sensors.append(s);
            }
            emit sensorsFetched(sensors);
        } else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    });
}

void AirQualityManager::fetchSensorData(int sensorId) {
    QUrl url(QString("https://api.gios.gov.pl/pjp-api/rest/data/getData/%1").arg(sensorId));
    QNetworkReply *reply = networkManager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QList<Measurement> measurements;
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = doc.object();
            QJsonArray values = obj["values"].toArray();
            for (const QJsonValue &value : values) {
                QJsonObject measurementObj = value.toObject();
                Measurement m;
                m.paramName = obj["key"].toString();
                m.value = measurementObj["value"].toDouble(-1.0);
                m.dateTime = QDateTime::fromString(measurementObj["date"].toString(), Qt::ISODate);
                measurements.append(m);
            }
            emit measurementsFetched(measurements);
        } else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    });
}

void AirQualityManager::fetchCoordinates(const QString &address) {
    QUrl url("https://nominatim.openstreetmap.org/search");
    QUrlQuery query;
    query.addQueryItem("q", address);
    query.addQueryItem("format", "json");
    query.addQueryItem("limit", "1");
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "AirQualityMonitor/1.0"); 

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray array = doc.array();
            if (!array.isEmpty()) {
                QJsonObject obj = array.first().toObject();
                double lat = obj["lat"].toString().toDouble();
                double lon = obj["lon"].toString().toDouble();
                emit coordinatesFetched(lat, lon);
            } else {
                emit errorOccurred("Nie znaleziono współrzędnych dla podanego adresu.");
            }
        } else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    });
}
