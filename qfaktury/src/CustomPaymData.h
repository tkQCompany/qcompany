/*
 * CustomPaymData.h
 *
 *  Created on: Apr 4, 2009
 *      Author: moux
 */

#ifndef CUSTOMPAYMDATA_H
#define CUSTOMPAYMDATA_H

#include <QString>
#include <QDate>
#include <QMetaType>

/**
 * @brief
 *
 */
class CustomPaymData
{
public:
    /**
     * @brief
     *
     */
    virtual ~CustomPaymData() {}

    QString payment1; /**< TODO */
    QDate date1; // or string? /**< TODO */
    double amount1; /**< TODO */

    QString payment2; /**< TODO */
    QDate date2; // or string? /**< TODO */
    double amount2; /**< TODO */
};

Q_DECLARE_METATYPE(CustomPaymData)

#endif
