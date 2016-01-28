#include "widget.h"
#include "Sender.h"

#include <QtWidgets>

#include <fstream>

constexpr double DEFAULT_LOT = 1.0;
constexpr const char DEFAULT_CURRENCY[] = "EURUSD";

void CreateMessage(char type, const std::string &cur, double lot);


Widget::Widget(QWidget *parent)
    : QWidget(parent), mLot(DEFAULT_LOT), mCurrency(DEFAULT_CURRENCY),
    mState(), mPrice(), mBuyPriceLabel(), mSellPriceLabel()
{
    // input
    QLabel* plotLabel = new QLabel("Lot:");
    QLabel* pcurrLabel = new QLabel("Currency:");
    QLabel* ppriceLabel = new QLabel("Price:");
    mLotEdit = new QLineEdit;
    QLineEdit* pcurrEdit = new QLineEdit;
    mPriceEdit = new QLineEdit;

    mBuyPriceLabel = new QLabel("0");
    mSellPriceLabel = new QLabel("0");
    QLabel* patLabel1 = new QLabel("at");
    QLabel* patLabel2 = new QLabel("at");

    mLotEdit->setMaximumWidth(40);
    pcurrEdit->setMaximumWidth(80);
    mPriceEdit->setMaximumWidth(60);

    // set lot validator
    auto lotValidator = new QDoubleValidator(0, 100, 2, mLotEdit);
    lotValidator->setNotation(QDoubleValidator::StandardNotation);
    mLotEdit->setValidator(lotValidator);

    auto priceValidator = new QDoubleValidator(0, 1000, 4, mPriceEdit);
    priceValidator->setNotation(QDoubleValidator::StandardNotation);
    mPriceEdit->setValidator(priceValidator);

    pcurrEdit->setReadOnly(true);
    pcurrEdit->setText(mCurrency);

    QFormLayout* pfLayout = new QFormLayout;
    pfLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    pfLayout->setHorizontalSpacing(20);
    pfLayout->setLabelAlignment(Qt::AlignRight);

    pfLayout->addRow(pcurrLabel, pcurrEdit);
    pfLayout->addRow(plotLabel, mLotEdit);
    pfLayout->addRow(ppriceLabel, mPriceEdit);

    // buttons
    QPushButton* pbuyBut = new QPushButton("Buy");
    QPushButton* psellBut = new QPushButton("Sell");
    QPushButton* prefresh = new QPushButton("Refresh");

    QHBoxLayout* pbuttonLayout = new QHBoxLayout;
    pbuttonLayout->addWidget(pbuyBut);
    pbuttonLayout->addSpacing(10);
    pbuttonLayout->addWidget(psellBut);

    QHBoxLayout* ppricesLayout = new QHBoxLayout;
    ppricesLayout->addWidget(patLabel1, 1);
    ppricesLayout->addWidget(mBuyPriceLabel, 3);
    ppricesLayout->addWidget(patLabel2, 1);
    ppricesLayout->addWidget(mSellPriceLabel, 3);

    // main layout
    QVBoxLayout* pmainLayout = new QVBoxLayout;
    pmainLayout->addLayout(pfLayout);
    pmainLayout->addSpacing(20);
    pmainLayout->addLayout(pbuttonLayout);
    pmainLayout->addLayout(ppricesLayout);
    pmainLayout->addWidget(prefresh);

    setLayout(pmainLayout);

    // connections
    connect(pbuyBut, SIGNAL(clicked()), SLOT(BuyButtonClicked()));
    connect(psellBut, SIGNAL(clicked()), SLOT(SellButtonClicked()));
    connect(prefresh, SIGNAL(clicked()), SLOT(RefreshButtonClicked()));
//    connect(mLotEdit, SIGNAL(editingFinished()), SLOT(SetLot()));



    mClient = new Client(SETTINGS_FILE_NAME);


    emit prefresh->clicked();
//    RefreshButtonClicked();
}

Widget::~Widget()
{
    delete mClient;
}

void Widget::SetBuyPrice(double buy_price) { mBuyPriceLabel->setNum(buy_price); }

void Widget::SetSellPrice(double sell_price) { mSellPriceLabel->setNum(sell_price); }

void Widget::BuyButtonClicked()
{
    if(!SetLot())
    {
        mLotEdit->setFocus();
        return;
    }
    if(!SetPrice())
    {
        mPriceEdit->setFocus();
        return;
    }
    mClient->Send('2', '1', mCurrency.toStdString(), mLot, mPrice);
//    CreateMessage('1', mCurrency.toStdString(), mLot);
}

void Widget::SellButtonClicked()
{
    if(!SetLot())
    {
        mLotEdit->setFocus();
        return;
    }
    if(!SetPrice())
    {
        mPriceEdit->setFocus();
        return;
    }
    mClient->Send('2', '2', mCurrency.toStdString(), mLot, mPrice);
//    CreateMessage('2', mCurrency.toStdString(), mLot);
}

void Widget::RefreshButtonClicked()
{
    std::function<void(double, double)> task(
        [this](double buy_price, double sell_price)
        {
            this->SetBuyPrice(buy_price);
            this->SetSellPrice(sell_price);
        }
    );

    mClient->AskForMarketData(std::move(task));
}

bool Widget::SetLot()
{
    if(!mLotEdit->hasAcceptableInput())
        return false;

    bool ok = false;
    mLot = mLotEdit->text().replace(',', '.').toDouble(&ok);

    return ok;
}

bool Widget::SetPrice()
{
    if(!mPriceEdit->hasAcceptableInput())
        return false;

    bool ok = false;
    mPrice = mPriceEdit->text().replace(',', '.').toDouble(&ok);

    return ok;
}

void CreateMessage(char type, const std::string& cur, double lot)
{
    std::ofstream file("order.dat");
    file << type << std::endl;
    file << cur << std::endl;
    file << lot << std::endl;
}
