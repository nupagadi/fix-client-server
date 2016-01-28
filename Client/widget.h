#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

constexpr char SETTINGS_FILE_NAME[] = "res/settings";

class QLineEdit;
class QLabel;
class Client;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    Widget(const Widget&) = delete;
    Widget& operator =(const Widget&) = delete;

    void SetBuyPrice(double buy_price);
    void SetSellPrice(double sell_price);

private:
    bool SetLot();
    bool SetPrice();

private slots:
    void BuyButtonClicked();
    void SellButtonClicked();    
    void RefreshButtonClicked();

private:
    double mLot;
    QString mCurrency;
    bool mState;
    double mPrice;

    QLineEdit* mLotEdit;
    QLineEdit* mPriceEdit;
    QLabel *mBuyPriceLabel;
    QLabel *mSellPriceLabel;

    Client* mClient;
};

#endif // WIDGET_H
