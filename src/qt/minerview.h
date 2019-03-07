// Copyright (c) 2019 The BitcoinV Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_MINERVIEW_H
#define BITCOIN_QT_MINERVIEW_H

#include <qt/guiutil.h>

#include <uint256.h>

#include <QWidget>
#include <QKeyEvent>

 #include <QMainWindow>
 #include <QPushButton>
 #include <QTextEdit>

class PlatformStyle;
class TransactionFilterProxy;
class WalletModel;

QT_BEGIN_NAMESPACE
class QComboBox;
class QDateTimeEdit;
class QFrame;
class QLineEdit;
class QMenu;
class QModelIndex;
class QSignalMapper;
class QTableView;
QT_END_NAMESPACE

/** Widget showing the transaction list for a wallet, including a filter row.
    Using the filter row, the user can view or export a subset of the transactions.
  */
class MinerView : public QWidget
{
    Q_OBJECT

public:
    explicit MinerView(const PlatformStyle *platformStyle, QWidget *parent = 0);

   void setModel(WalletModel *model);

    // Date ranges for filter
    enum Mining_Type_t
    {
        BLOCK_REWARD_X1,
        BLOCK_REWARD_X2,
        BLOCK_REWARD_X4,
        BLOCK_REWARD_X8,
        BLOCK_REWARD_X16,
        BLOCK_REWARD_X32,
        BLOCK_REWARD_X64,
        BLOCK_REWARD_X128,
        BLOCK_REWARD_X256,
        BLOCK_REWARD_X512,
        BLOCK_REWARD_X1024,
        BLOCK_REWARD_X2048,
        BLOCK_REWARD_X4096,
        BLOCK_REWARD_X8192,
        BLOCK_REWARD_X16384,
        BLOCK_REWARD_X32768,
        BLOCK_REWARD_X65536,
        BLOCK_REWARD_X131072,
        BLOCK_REWARD_X262144,
        BLOCK_REWARD_X524288,
        BLOCK_REWARD_X1048576,
    };



 private Q_SLOTS:

    void handleRewardMultiplier(int);


 private:
 
    QTextEdit *m_minerView;

    QPushButton *m_button;
    bool m_mining_active;

    QComboBox *m_mine_type;

    Mining_Type_t m_mining_type;

};

#endif // BITCOIN_QT_MINERVIEW_H