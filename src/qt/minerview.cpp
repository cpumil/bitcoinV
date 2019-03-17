// Copyright (c) 2019 The BitcoinV Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/minerview.h>

#include <qt/addresstablemodel.h>
#include <qt/bitcoinunits.h>
#include <qt/csvmodelwriter.h>
#include <qt/editaddressdialog.h>
#include <qt/optionsmodel.h>
#include <qt/platformstyle.h>
#include <qt/sendcoinsdialog.h>
#include <qt/transactiondescdialog.h>
#include <qt/transactionfilterproxy.h>
#include <qt/transactionrecord.h>
#include <qt/transactiontablemodel.h>
#include <qt/walletmodel.h>

#include <ui_interface.h>
#include <variable_block_reward.h>

#include <QComboBox>
#include <QDateTimeEdit>
#include <QDesktopServices>
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPoint>
#include <QScrollBar>
#include <QSignalMapper>
#include <QTableView>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>
#include <QTextEdit>

#include <rpc/server.h>
#include <rpc/client.h>
#include <univalue/include/univalue.h>

#include <pthread.h>
#include <stdio.h>
#include <atomic>
#include <thread>
#include <unistd.h>
#include <iostream>

// variable block rewards, defaults to 1 in miner.cpp
extern uint64_t g_extra_multiply;

MinerView::MinerView(const PlatformStyle *platformStyle, QWidget *parent) :
    QWidget(parent), m_minerView(0), m_mining_active{false},
    m_mining_type{BLOCK_REWARD_X1}
{
    // Build filter row
    setContentsMargins(0,0,0,0);

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setContentsMargins(0,0,0,0);

    if (platformStyle->getUseExtraSpacing()) {
        hlayout->setSpacing(5);
        hlayout->addSpacing(26);
    } else {
        hlayout->setSpacing(0);
        hlayout->addSpacing(23);
    }


    m_mine_type = new QComboBox(this);
    if (platformStyle->getUseExtraSpacing()) {
        m_mine_type->setFixedWidth(251);
    } else {
        m_mine_type->setFixedWidth(250);
    }

    m_mine_type->addItem(tr("MINE BLOCK REWARD x 1"), BLOCK_REWARD_X1);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 2"), BLOCK_REWARD_X2);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 4"), BLOCK_REWARD_X4);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 8"), BLOCK_REWARD_X8);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 16"), BLOCK_REWARD_X16);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 32"), BLOCK_REWARD_X32);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 64"), BLOCK_REWARD_X64);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 128"), BLOCK_REWARD_X128);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 256"), BLOCK_REWARD_X256);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 512"), BLOCK_REWARD_X512);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 1024"), BLOCK_REWARD_X1024);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 2048"), BLOCK_REWARD_X2048);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 4096"), BLOCK_REWARD_X4096);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 8192"), BLOCK_REWARD_X8192);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 16384"), BLOCK_REWARD_X16384);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 32768"), BLOCK_REWARD_X32768);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 65536"), BLOCK_REWARD_X65536);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 131072"), BLOCK_REWARD_X131072);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 262144"), BLOCK_REWARD_X262144);
    m_mine_type->addItem(tr("MINE BLOCK REWARD x 524288"), BLOCK_REWARD_X524288);
    m_mine_type->addItem(tr("MINE JACKPOT BLOCK REWARD"), BLOCK_REWARD_X1048576);


    hlayout->addWidget(m_mine_type);


    // Delay before filtering transactions in ms
    static const int input_filter_delay = 200;


    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(0);

    m_minerView = new QTextEdit();


    vlayout->addLayout(hlayout);
    vlayout->addWidget(m_minerView);
    vlayout->setSpacing(0);


    m_minerView->installEventFilter(this);

    m_minerView->setObjectName("minerView");

    connect(m_mine_type, SIGNAL (activated(int)), this, SLOT (handleRewardMultiplier(int)));

}

// Handle the specified reward.
void MinerView::handleRewardMultiplier(int pos)
{
    int32_t mult_fact = 1 << pos;
    g_extra_multiply = floor_power_2_vbr(mult_fact);
    return;
}
