#ifndef INVOICENUMBERFORMAT_T_H
#define INVOICENUMBERFORMAT_T_H

#include <QObject>
#include <QStringList>
#include <QMetaType>
#include <utility>
#include <memory>

#include "SettingsGlobal.h"

class InvoiceNumberFormat_t: public QObject
{
    Q_OBJECT
public:
    enum Field {NR, NR_Y, NR_M, NR_D, NR_Q, INVOICE_TYPE, TEXT1, TEXT2, TEXT3,
                PERIOD_YEAR, PERIOD_SHORT_YEAR, PERIOD_MONTH, PERIOD_DAY, PERIOD_QUARTER, SLASH, BACKSLASH, HYPHEN};
    enum Separator {SEPARATOR_START = SLASH, SEPARATOR_END = HYPHEN};

    explicit InvoiceNumberFormat_t(QObject *parent = 0);

    bool append(const Field field, const SettingsGlobal &s);
    QString cap(const QString &strIn, const int position) const;
    void clearAll();

    QList<Field> fieldList() const;
    QStringList fieldStrList() const;
    bool isEmpty() const {return list_.isEmpty();}

    static QString FieldName(const InvoiceNumberFormat_t::Field field);

    static Field FieldID(const QString &field);

    static QString FieldDescription(const Field field);

    int size() const {return list_.size();}

    static QString toRegexp(const QString& format);
    QString toString() const;

    static std::auto_ptr<InvoiceNumberFormat_t> Parse(const QString &format);

private:
    static QString fieldToRegexpStr(const Field field, const SettingsGlobal &s);
    
private:
    typedef Field pairFirstType_;
    typedef QString pairSecondType_;
    QList<std::pair<pairFirstType_, pairSecondType_> > list_;
    QRegExp regexp_;
};

Q_DECLARE_METATYPE(InvoiceNumberFormat_t::Field)

#endif // INVOICENUMBERFORMAT_T_H
