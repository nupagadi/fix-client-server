#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

constexpr char SETTINGS_FILE_NAME[] = "res/settings";

class QLineEdit;
class Client;
class QRadioButton;
class QPushButton;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    Widget(const Widget&) = delete;
    Widget& operator =(const Widget&) = delete;

    void SetBuyButtonText(double buy_price/* = 0*/);
    void SetSellButtonText(double sell_price/* = 0*/);

private:
    bool SetLot();
    bool SetPrice();
    char GetOrderType();

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

    QPushButton* mBuyButton;
    QPushButton* mSellButton;

    QRadioButton* mRadMarket;
    QRadioButton* mRadLimit;
    QRadioButton* mRadStop;

    Client* mClient;
};

#endif // WIDGET_H
