#include "widget.h"
#include "Sender.h"

#include <QtWidgets>

#include <fstream>

constexpr double DEFAULT_LOT = 1.0;
constexpr const char DEFAULT_CURRENCY[] = "EURUSD";

void CreateMessage(char type, const std::string &cur, double lot);


Widget::Widget(QWidget *parent)
    : QWidget(parent), mLot(DEFAULT_LOT), mCurrency(DEFAULT_CURRENCY),
    mState(), mPrice(), mBid(), mOffer()
{
    // form
    QLabel* plotLabel = new QLabel("Lot:");
    QLabel* pcurrLabel = new QLabel("Currency:");
    QLabel* ppriceLabel = new QLabel("Price:");
    mLotEdit = new QLineEdit;
    QLineEdit* pcurrEdit = new QLineEdit;
    mPriceEdit = new QLineEdit;

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
//    pfLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    pfLayout->setHorizontalSpacing(20);
    pfLayout->setLabelAlignment(Qt::AlignRight);

    pfLayout->addRow(pcurrLabel, pcurrEdit);
    pfLayout->addRow(plotLabel, mLotEdit);
    pfLayout->addRow(ppriceLabel, mPriceEdit);


    // buttons
    mBuyButton = new QPushButton("Buy");
    mSellButton = new QPushButton("Sell");
    QPushButton* prefresh = new QPushButton("Refresh");

    QHBoxLayout* pbuttonLayout = new QHBoxLayout;
    pbuttonLayout->addWidget(mBuyButton);
    pbuttonLayout->addSpacing(10);
    pbuttonLayout->addWidget(mSellButton);


    // radio buttons    
    mRadMarket = new QRadioButton("Market");
    mRadLimit = new QRadioButton("Limit");
    mRadStop = new QRadioButton("Stop");
    mRadMarket->setChecked(true);
    QVBoxLayout* pradioLayout = new QVBoxLayout;
    pradioLayout->addWidget(mRadMarket);
    pradioLayout->addWidget(mRadLimit);
    pradioLayout->addWidget(mRadStop);

//    QGroupBox* pradioGroup = new QGroupBox;
//    pradioGroup->setFlat(true);
//    pradioGroup->setLayout(pradioLayout);

    QGridLayout* pmainGrid = new QGridLayout;
    pmainGrid->addLayout(pfLayout, 0, 0);
    pmainGrid->addLayout(pradioLayout, 0, 1);
    pmainGrid->addLayout(pbuttonLayout, 1, 0, 1, 2);
    pmainGrid->addWidget(prefresh, 2, 0, 1, 2);

    pmainGrid->setHorizontalSpacing(30);
    pmainGrid->setVerticalSpacing(15);
    pmainGrid->setMargin(15);

    setLayout(pmainGrid);


    // connections
    connect(mBuyButton, SIGNAL(clicked()), SLOT(BuyButtonClicked()));
    connect(mSellButton, SIGNAL(clicked()), SLOT(SellButtonClicked()));
    connect(prefresh, SIGNAL(clicked()), SLOT(RefreshButtonClicked()));
//    connect(mLotEdit, SIGNAL(editingFinished()), SLOT(SetLot()));

    connect(mRadMarket, SIGNAL(clicked()), SLOT(RadioButtonClicked()));
    connect(mRadLimit, SIGNAL(clicked()), SLOT(RadioButtonClicked()));
    connect(mRadStop, SIGNAL(clicked()), SLOT(RadioButtonClicked()));


    mClient = new Client(SETTINGS_FILE_NAME);


    emit prefresh->clicked();
//    RefreshButtonClicked();
}

Widget::~Widget()
{
    delete mClient;
}

void Widget::SetBuyButtonText(double buy_price = 0)
{
    QString buttonText("Buy");
    // qFuzzyCompare() has some zero compare issues
    if(GetOrderType() == '1' && !qFuzzyCompare(buy_price+1, 1))
        buttonText += " at " + QString::number(buy_price);
    mBuyButton->setText(buttonText);
}

void Widget::SetSellButtonText(double sell_price = 0)
{
    QString buttonText("Sell");
    if(GetOrderType() == '1' && !qFuzzyCompare(sell_price+1, 1))
        buttonText += " at " + QString::number(sell_price);
    mSellButton->setText(buttonText);
}

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
    mClient->Send(GetOrderType(), '1', mCurrency.toStdString(), mLot, mPrice);
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
    mClient->Send(GetOrderType(), '2', mCurrency.toStdString(), mLot, mPrice);
//    CreateMessage('2', mCurrency.toStdString(), mLot);
}

void Widget::RefreshButtonClicked()
{
    std::function<void(double, double)> task(
        [this](double bid, double offer)
        {
            this->SetBid(bid);
            this->SetOffer(offer);
        }
    );

    mClient->AskForMarketData(std::move(task));


    RadioButtonClicked();
}

void Widget::RadioButtonClicked()
{
    QString buyButtonText("Buy"), sellButtonText("Sell");
    // qFuzzyCompare() has some zero compare issues
    if(GetOrderType() == '1' && !qFuzzyCompare(mBid+1, 1) && !qFuzzyCompare(mOffer+1, 1))
    {
        buyButtonText += " at " + QString::number(mBid);
        sellButtonText += " at " + QString::number(mOffer);
    }
    mBuyButton->setText(buyButtonText);
    mSellButton->setText(sellButtonText);
}

void Widget::SetOffer(double offer)
{
    mOffer = offer;
}

void Widget::SetBid(double bid)
{
    mBid = bid;
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

char Widget::GetOrderType()
{
    if(mRadMarket->isChecked())
        return '1';
    else if(mRadLimit->isChecked())
        return '2';
    else if(mRadStop->isChecked())
        return '3';
    else throw std::logic_error("Wrong radio button state");
}

void CreateMessage(char type, const std::string& cur, double lot)
{
    std::ofstream file("order.dat");
    file << type << std::endl;
    file << cur << std::endl;
    file << lot << std::endl;
}
