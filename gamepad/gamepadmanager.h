#pragma once

#include <QDebug>
#include <QObject>
#include <QString>
#include <QTimer>

struct _SDL_GameController;
typedef _SDL_GameController SDL_GameController;

class GamepadManager: public QObject{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(double leftX READ leftX NOTIFY axesChanged)
    Q_PROPERTY(double leftY READ leftY NOTIFY axesChanged)
    Q_PROPERTY(double rightX READ rightX NOTIFY axesChanged)
    Q_PROPERTY(double rightY READ rightY NOTIFY axesChanged)

public:
    GamepadManager(QObject* parent = nullptr);
    ~GamepadManager() override;

    bool connected() {return _isConnected;}
    QString name() {return _name;}

    double leftY() const {return _leftY;}
    double rightY() const {return _rightY;}
    double leftX() const {return _leftX;}
    double rightX() const {return _rightX;}

signals:
    void connectedChanged();
    void nameChanged();
    void axesChanged();
private slots:
    void poll();
private:
    void openController();
    void closeController();
    static double normalizeAxis(int raw);

    bool _isConnected{false};
    QString _name;
    double _leftX{0.};
    double _rightX{0.};
    double _leftY{0.};
    double _rightY{0.};

    QTimer* timer{nullptr};

    SDL_GameController* controller{nullptr};
};
