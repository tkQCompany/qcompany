#include "Database.h"

Database::Database(QObject *parent): QObject(parent)
{
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(qApp->applicationName() + "-" + qApp->applicationVersion() + ".db3");
    db_.setHostName("localhost");
    if(!db_.open())
    {
        QMessageBox::critical(0, trUtf8("Błąd DB"), trUtf8("Nie mogę otworzyć bazy danych"));
        return;
    }

    db_.exec("PRAGMA foreign_keys = ON");

    if(!createTablesIfNotExist())
    {                    
        exit(EXIT_FAILURE);
    }

    if(!insertDataIfNotInserted())
    {
        exit(EXIT_FAILURE);
    }

    initModels();
}

Database::~Database()
{
    const QString connName(db_.connectionName());
    db_.close();
    delete modelCommodity_;
    delete modelCommodityType_;
    delete modelCounterparty_;
    delete modelCounterpartyType_;
    delete modelCountry_;
    delete modelCurrency_;
    delete modelEmail_;
    delete modelInvoice_;
    delete modelInvoiceType_;
    delete modelInvoiceWithCommodities_;
    delete modelPaymentType_;
    delete modelPhone_;
    delete modelUnit_;
    delete modelVat_;
    db_ = QSqlDatabase();
    db_.removeDatabase(connName);
}


const QString Database::dbFileName() const
{
    return db_.databaseName();
}



ModelCommodity* Database::modelCommodity() const
{
    return modelCommodity_;
}


ModelCommodityType* Database::modelCommodityType() const
{
    return modelCommodityType_;
}

ModelCounterparty* Database::modelCounterparty() const
{
    return modelCounterparty_;
}


ModelCounterpartyType* Database::modelCounterpartyType() const
{
    return modelCounterpartyType_;
}


ModelCountry* Database::modelCountry() const
{
    return modelCountry_;
}


ModelCurrency* Database::modelCurrency() const
{
    return modelCurrency_;
}


ModelEmail* Database::modelEmail() const
{
    return modelEmail_;
}


ModelInvoice* Database::modelInvoice() const
{
    return modelInvoice_;
}


ModelInvoiceType* Database::modelInvoiceType() const
{
    return modelInvoiceType_;
}


ModelInvoiceWithCommodities* Database::modelInvoiceWithCommodities() const
{
    return modelInvoiceWithCommodities_;
}


ModelPaymentType* Database::modelPaymentType() const
{
    return modelPaymentType_;
}


ModelPhone* Database::modelPhone() const
{
    return modelPhone_;
}


ModelUnit* Database::modelUnit() const
{
    return modelUnit_;
}


ModelVat* Database::modelVat() const
{
    return modelVat_;
}


bool Database::createTablesIfNotExist()
{
    if(!sqlExecute(QString("CREATE TABLE IF NOT EXISTS `%1` ("
               "`id_country` INTEGER PRIMARY KEY AUTOINCREMENT,"
               "`name` VARCHAR(80) NOT NULL UNIQUE)").arg("country"), __LINE__))
    {
        return false;
    }

    if(!sqlExecute(QString("CREATE  TABLE IF NOT EXISTS `%1` ("
                           "`id_counterparty_type` INTEGER PRIMARY KEY AUTOINCREMENT ,"
                           "`type` VARCHAR(45) NOT NULL UNIQUE)"
                           ).arg("counterparty_type"), __LINE__))
    {
        return false;
    }


    if(!sqlExecute(QString("CREATE TABLE IF NOT EXISTS `%1` ("
                           "`id_counterparty` INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "`name` VARCHAR(45) NOT NULL UNIQUE,"
                           "`type_id` INTEGER NOT NULL ,"
                           "`country_id` INTEGER NOT NULL ,"
                           "`location` VARCHAR(45) NOT NULL ,"
                           "`postal_code` VARCHAR(10) NOT NULL ,"
                           "`street` VARCHAR(45) NOT NULL ,"
                           "`tax_ident` VARCHAR(25) NOT NULL ,"
                           "`account_name` VARCHAR(25) NOT NULL ,"
                           "`www` VARCHAR(45) NOT NULL ,"
                           "`primary_email` VARCHAR(45) NOT NULL,"
                           "`primary_phone` VARCHAR(45) NOT NULL,"
                           "`inv_number_format` VARCHAR(100) NOT NULL,"
                           "CONSTRAINT `country_id` "
                             "FOREIGN KEY (`country_id` ) "
                             "REFERENCES `country` (`id_country` )"
                             "ON DELETE RESTRICT "
                             "ON UPDATE CASCADE,"
                           "CONSTRAINT `type_id` "
                             "FOREIGN KEY (`type_id` ) "
                             "REFERENCES `counterparty_type` (`id_counterparty_type` )"
                             "ON DELETE RESTRICT "
                           "ON UPDATE CASCADE)").arg("counterparty"), __LINE__))
    {
        return false;
    }

    if(!sqlExecute(QString("CREATE TABLE IF NOT EXISTS `%1` ("
                           "`id_phone` INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "`counterparty_id` INTEGER NOT NULL,"
                           "`number` VARCHAR(30) NOT NULL UNIQUE,"
                           "CONSTRAINT `fk_phone_counterparty` "
                           "FOREIGN KEY (`counterparty_id` ) "
                           "REFERENCES `counterparty` (`id_counterparty` ) "
                           "ON DELETE RESTRICT "
                           "ON UPDATE CASCADE)"
                       ).arg("additional_phone"), __LINE__))
    {
        return false;
    }

    if(!sqlExecute(QString("CREATE TABLE IF NOT EXISTS `%1` ("
               "`id_email` INTEGER PRIMARY KEY AUTOINCREMENT,"
               "`counterparty_id` INTEGER NOT NULL,"
               "`email` VARCHAR(45) NOT NULL UNIQUE, "
               "CONSTRAINT `fk_email_counterparty` "
               "FOREIGN KEY (`counterparty_id` ) "
               "REFERENCES `counterparty` (`id_counterparty` ) "
               "ON DELETE RESTRICT "
               "ON UPDATE CASCADE)"
               ).arg("additional_email"), __LINE__))
    {
        return false;
    }



    if(!sqlExecute(QString("CREATE TABLE IF NOT EXISTS `%1` ("
               "id_unit INTEGER PRIMARY KEY AUTOINCREMENT ,"
               "name VARCHAR(15) NOT NULL UNIQUE)"
                           ).arg("unit"), __LINE__))
    {
        return false;
    }


    if(!sqlExecute(QString("CREATE TABLE IF NOT EXISTS `%1` ("
               "id_commodity_type INTEGER PRIMARY KEY AUTOINCREMENT ,"
               "type VARCHAR(45) NOT NULL UNIQUE)"
               ).arg("commodity_type"), __LINE__))
    {
        return false;
    }



    if(!sqlExecute(QString("CREATE TABLE IF NOT EXISTS `%1` ("
                           "id_commodity INTEGER PRIMARY KEY AUTOINCREMENT ,"
                           "name VARCHAR(100) NOT NULL UNIQUE,"
                           "abbreviation VARCHAR(45) NOT NULL ,"
                           "pkwiu VARCHAR(25) NOT NULL ,"
                           "type_id INTEGER NOT NULL,"
                           "unit_id INTEGER NOT NULL,"
                           "net1 DECIMAL(10,2) NOT NULL DEFAULT 0,"
                           "net2 DECIMAL(10,2) NOT NULL DEFAULT 0,"
                           "net3 DECIMAL(10,2) NOT NULL DEFAULT 0,"
                           "net4 DECIMAL(10,2) NOT NULL DEFAULT 0,"
                           "vat VARCHAR(10) NOT NULL,"
                           "quantity INTEGER NOT NULL DEFAULT 0,"
                           "CONSTRAINT id_type_fk "
                             "FOREIGN KEY (type_id) "
                             "REFERENCES commodity_type (id_commodity_type) "
                             "ON DELETE RESTRICT "
                             "ON UPDATE CASCADE,"
                           "CONSTRAINT id_unit_fk "
                             "FOREIGN KEY (unit_id) "
                             "REFERENCES unit(id_unit) "
                             "ON DELETE RESTRICT "
                             "ON UPDATE CASCADE)"
                           ).arg("commodity"), __LINE__))
    {
        return false;
    }

    if(!sqlExecute(QString("CREATE  TABLE IF NOT EXISTS `%1` ("
               "`id_currency` INTEGER PRIMARY KEY AUTOINCREMENT,"
               "`name` VARCHAR(25) NOT NULL UNIQUE)"
               ).arg("currency"), __LINE__))
    {
        return false;
    }


    if(!sqlExecute(QString("CREATE  TABLE IF NOT EXISTS `%1` ("
                           "`id_invoice_type` INTEGER PRIMARY KEY NOT NULL ,"
                           "`type` VARCHAR(20) NOT NULL UNIQUE)")
                   .arg("invoice_type"), __LINE__))
    {
        return false;
    }


    if(!sqlExecute(QString("CREATE  TABLE IF NOT EXISTS `%1` ("
                           "`id_payment_type` INTEGER PRIMARY KEY NOT NULL ,"
                           "`type` VARCHAR(45) NOT NULL UNIQUE)")
                   .arg("payment_type"), __LINE__))
    {
        return false;
    }


    if(!sqlExecute(QString("CREATE  TABLE IF NOT EXISTS `%1` ("
                           "`id_invoice` INTEGER PRIMARY KEY AUTOINCREMENT ,"
                           "`inv_number` VARCHAR(100) NOT NULL UNIQUE,"
                           "`selling_date` DATE NOT NULL ,"
                           "`type_id` INTEGER NOT NULL ,"
                           "`counterparty_id` INTEGER NOT NULL ,"
                           "`issuance_date` DATE NOT NULL ,"
                           "`payment_date` DATE NOT NULL ,"
                           "`payment_id` INTEGER NOT NULL ,"
                           "`currency_id` INTEGER NOT NULL ,"
                           "`additional_text` VARCHAR(200) NOT NULL ,"
                           "`discount` INTEGER NOT NULL ,"
                           "CONSTRAINT `type_id`"
                             "FOREIGN KEY (`type_id` )"
                             "REFERENCES `invoice_type` (`id_invoice_type` ) "
                             "ON DELETE RESTRICT "
                             "ON UPDATE CASCADE,"
                           "CONSTRAINT `counterparty_id`"
                             "FOREIGN KEY (`counterparty_id` )"
                             "REFERENCES `counterparty` (`id_counterparty` ) "
                             "ON DELETE RESTRICT "
                             "ON UPDATE CASCADE,"
                           "CONSTRAINT `payment_id`"
                             "FOREIGN KEY (`payment_id` )"
                             "REFERENCES `payment_type` (`id_payment_type` ) "
                             "ON DELETE RESTRICT "
                             "ON UPDATE CASCADE,"
                           "CONSTRAINT `currency_id` "
                             "FOREIGN KEY (`currency_id` ) "
                             "REFERENCES `currency` (`id_currency` ) "
                             "ON DELETE RESTRICT "
                             "ON UPDATE CASCADE)")
                   .arg("invoice"), __LINE__))
    {
        return false;
    }

    if(!sqlExecute(QString("CREATE  TABLE IF NOT EXISTS `%1` ("
                           "`id` INTEGER PRIMARY KEY NOT NULL ,"
                           "`invoice_id` INTEGER NOT NULL,"
                           "`commodity_id` INTEGER NOT NULL,"
                           "`net` DECIMAL(10,2) NOT NULL,"
                           "`quantity` INTEGER NOT NULL,"
                           "`discount` INTEGER NOT NULL,"
                           "CONSTRAINT `invoice_id`"
                             "FOREIGN KEY (`invoice_id`) "
                             "REFERENCES `invoice` (`id_invoice`) "
                             "ON DELETE CASCADE "
                             "ON UPDATE CASCADE,"
                           "CONSTRAINT `commodity_id`"
                             "FOREIGN KEY (`commodity_id`) "
                             "REFERENCES `commodity` (`id_commodity`) "
                             "ON DELETE RESTRICT "
                             "ON UPDATE CASCADE)"
                           ).arg("table_invoice_commodity"), __LINE__))
    {
        return false;
    }

    return true;
}


bool Database::sqlExecute(const QString &sqlQuery, const int line)
{
    QSqlQuery query(db_);

    query.exec(sqlQuery);
    if(!query.isActive())
    {
        QMessageBox::critical(0, trUtf8("Błąd SQL"), QString("Detected at line %1: %2").arg(line).arg(query.lastError().text()));
        return false;
    }
    return true;
}

bool Database::sqlInsertIf(const QString &table, const QString &column, const QString &condition, const int line)
{
    bool ret = true;
    QSqlQuery query(db_);
    db_.transaction();

    query.exec(QString("SELECT `%1` FROM `%2` WHERE `%1` = \"%3\"").arg(column).arg(table).arg(condition));
    if(!query.isActive())
    {
        QMessageBox::critical(0, trUtf8("Błąd SQL SELECT"), QString("Detected at line %1: %2").arg(line).arg(query.lastError().text()));
        ret = false;
    }
    else
    {
         if(!query.next())
         {
             if(!sqlExecute(QString("INSERT OR IGNORE INTO `%1`(`%2`) VALUES('%3')").arg(table).arg(column).arg(condition), line))
             {
                 QMessageBox::critical(0, trUtf8("Błąd SQL INSERT"), QString("Detected at line %1: %2").arg(line).arg(query.lastError().text()));
                 ret = false;
             }
         }
    }
    db_.commit();
    return ret;
}


void Database::initModels()
{
    modelCommodityType_ = new ModelCommodityType(this->parent());
    modelCommodityType_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelCommodityType_->setSort(CommodityTypeFields::ID_COMMODITY_TYPE, Qt::AscendingOrder);
    modelCommodityType_->select();

    modelCountry_ = new ModelCountry(this->parent());
    modelCountry_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelCountry_->setSort(CountryFields::ID_COUNTRY, Qt::AscendingOrder);
    modelCountry_->select();

    modelEmail_ = new ModelEmail(this->parent(), "");
    modelEmail_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelEmail_->setSort(EmailFields::ID_EMAIL, Qt::AscendingOrder);
    modelEmail_->setRelation(EmailFields::COUNTERPARTY_ID, QSqlRelation("counterparty", "id_counterparty", "name"));
    modelEmail_->select();

    modelPhone_ = new ModelPhone(this->parent());
    modelPhone_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelPhone_->setSort(PhoneFields::ID_PHONE, Qt::AscendingOrder);
    modelPhone_->setRelation(PhoneFields::COUNTERPARTY_ID, QSqlRelation("counterparty", "id_counterparty", "name"));
    modelPhone_->select();

    modelUnit_ = new ModelUnit(this->parent());
    modelUnit_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelUnit_->setSort(UnitFields::ID_UNIT, Qt::AscendingOrder);
    modelUnit_->select();

    modelVat_ = new ModelVat(this->parent());

    modelCommodity_ = new ModelCommodity(this->parent());
    modelCommodity_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelCommodity_->setSort(CommodityFields::ID, Qt::AscendingOrder);
    modelCommodity_->setRelation(CommodityFields::TYPE_ID, QSqlRelation("commodity_type", "id_commodity_type", "type"));
    modelCommodity_->setRelation(CommodityFields::UNIT_ID, QSqlRelation("unit", "id_unit", "name"));
    modelCommodity_->select();

    modelCounterpartyType_ = new ModelCounterpartyType(this->parent());
    modelCounterpartyType_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelCounterpartyType_->setSort(CounterpartyTypeFields::ID, Qt::AscendingOrder);
    modelCounterpartyType_->select();

    modelCounterparty_ = new ModelCounterparty(this->parent());
    modelCounterparty_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelCounterparty_->setSort(CounterpartyFields::ID, Qt::AscendingOrder);
    modelCounterparty_->setRelation(CounterpartyFields::COUNTRY_ID, QSqlRelation("country", "id_country", "name"));
    modelCounterparty_->setRelation(CounterpartyFields::TYPE_ID, QSqlRelation("counterparty_type", "id_counterparty_type", "type"));
    modelCounterparty_->setFilter(QString("type_id != %1").arg(CounterpartyTypeData::MY_COMPANY));
    modelCounterparty_->select();

    modelInvoice_ = new ModelInvoice(this->parent());
    modelInvoice_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelInvoice_->setSort(InvoiceFields::ID_INVOICE, Qt::AscendingOrder);
    modelInvoice_->setRelation(InvoiceFields::TYPE_ID, QSqlRelation("invoice_type", "id_invoice_type", "type"));
    modelInvoice_->setRelation(InvoiceFields::COUNTERPARTY_ID, QSqlRelation("counterparty", "id_counterparty", "name"));
    modelInvoice_->setRelation(InvoiceFields::PAYMENT_ID, QSqlRelation("payment_type", "id_payment_type", "type"));
    modelInvoice_->setRelation(InvoiceFields::CURRENCY_ID, QSqlRelation("currency", "id_currency", "name"));
    modelInvoice_->select();

    modelCurrency_ = new ModelCurrency(this->parent());
    modelCurrency_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelCurrency_->setSort(CurrencyFields::ID_CURRENCY, Qt::AscendingOrder);
    modelCurrency_->select();

    modelPaymentType_ = new ModelPaymentType(this->parent());
    modelPaymentType_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelPaymentType_->setSort(PaymentTypeFields::ID_PAYMENT_TYPE, Qt::AscendingOrder);
    modelPaymentType_->select();

    modelInvoiceType_ = new ModelInvoiceType(this->parent());
    modelInvoiceWithCommodities_ = new ModelInvoiceWithCommodities(this->parent());

    if(!db_.isValid())
    {
        qDebug() << "Database::initModels(): db: " << db_.lastError().text();
    }
}



bool Database::insertDataIfNotInserted()
{
    for(int i = CounterpartyTypeData::MY_COMPANY; i <= CounterpartyTypeData::OFFICE; ++i)
    {
        if(!sqlInsertIf("counterparty_type", "type", CounterpartyTypeData::names(i), __LINE__))
            return false;
    }

    if(!sqlInsertIf("country", "name", DB_Constants::My_Country, __LINE__))
        return false;

    for(int i = CommodityTypeData::GOODS; i <= CommodityTypeData::SERVICES; ++i)
    {
        if(!sqlInsertIf("commodity_type", "type", CommodityTypeData::names(i), __LINE__))
            return false;
    }

    for(int i = UnitData::UNIT; i <= UnitData::PACKAGE; ++i)
    {
        if(!sqlInsertIf("unit", "name", UnitData::name(i), __LINE__))
            return false;
    }

    for(int i = CurrencyData::EUR; i <= CurrencyData::USD; ++i)
    {
        if(!sqlInsertIf("currency", "name", CurrencyData::name(i), __LINE__))
            return false;
    }

    for(int i = PaymentTypeData::CASH; i <= PaymentTypeData::TRANSFER; ++i)
    {
        if(!sqlInsertIf("payment_type", "type", PaymentTypeData::name(i), __LINE__))
            return false;
    }

    for(int i = InvoiceTypeData::VAT; i <= InvoiceTypeData::BILL; ++i)
    {
        if(!sqlInsertIf("invoice_type", "type", InvoiceTypeData::InvoiceTypeToString(i), __LINE__))
            return false;
    }

    QSqlQuery query(db_);
    query.exec(QString("SELECT `name` FROM `counterparty` LEFT JOIN `counterparty_type` ON `type_id` = `id_counterparty_type`"));
    if(!query.isActive())
    {
        QMessageBox::critical(0, trUtf8("Błąd SQL SELECT"), QString("Detected at line %1: %2").arg(__LINE__).arg(query.lastError().text()));
        return false;
    }
    if(!query.next())
    {
        const QString selectCountry(QString("(SELECT `id_country` FROM `country` WHERE `name` = \"%1\")").arg(DB_Constants::My_Country));
        query.exec(QString("INSERT INTO `counterparty`(`name`, `type_id`, `country_id`, `location`, `postal_code`, "
                           "`street`, `tax_ident`, `account_name`, `www`, `primary_email`, `primary_phone`, 'inv_number_format') "
                   "VALUES(\"%1\", %2, %3, \"%4\", \"%5\", \"%6\", \"%7\", \"%8\", \"%9\", \"%10\", \"%11\", \"%12\")")
                   .arg(trUtf8("Brak nazwy firmy")).arg(CounterpartyTypeData::MY_COMPANY).arg(selectCountry)
                   .arg(trUtf8("Brak nazwy miejscowości")).arg(trUtf8("Brak kodu pocztowego")).arg(trUtf8("Brak nazwy ulicy"))
                   .arg(trUtf8("Brak NIP")).arg(trUtf8("Brak numeru konta")).arg("").arg("").arg("").arg(""));
        if(!query.isActive())
        {
            QMessageBox::critical(0, trUtf8("Błąd SQL INSERT"), QString("Detected at line %1: %2\nQuery:\n%3")
                                  .arg(__LINE__).arg(query.lastError().text())
                                  .arg(query.lastQuery()));
            return false;
        }
    }

    return true;
}



bool Database::invoiceWithCommoditiesInsertTransact(const InvoiceData &invoice, const QList<CommodityVisualData> &commodities)
{
    QString errorMsg;
    if(commodities.isEmpty())
    {
        errorMsg = "List of commodities is empty. Empty invoices are not allowed.";
    }
    else
    {
        if(db_.transaction())
        {
            QSqlQuery queryInv(modelInvoice()->query());
            const QString dateFormat("yyyy-MM-dd");
            const QString queryInvStr(QString("INSERT INTO invoice(inv_number, selling_date, type_id, counterparty_id, issuance_date, payment_date, payment_id, currency_id, additional_text, discount) VALUES('%1', '%2', %3, %4, '%5', '%6', %7, %8, '%9', %10)")
                                      .arg(invoice.invNumber).arg(invoice.sellingDate.toString(dateFormat)).arg(invoice.typeID)
                                      .arg(invoice.counterpartyID).arg(invoice.issuanceDate.toString(dateFormat))
                                      .arg(invoice.paymentDate.toString(dateFormat)).arg(invoice.paymentID).arg(invoice.currencyID)
                                      .arg(invoice.additText).arg(invoice.discount));

            if(queryInv.exec(queryInvStr))
            {
                const qint64 id_invoice = queryInv.lastInsertId().toLongLong();

                QSqlQuery queryInvCommod(modelInvoiceWithCommodities_->query());
                const QString queryInvCommodStr("INSERT INTO table_invoice_commodity(invoice_id, commodity_id, net, quantity, discount) VALUES (:invoice_id, :commodity_id, :net, :quantity, :discount)");
                queryInvCommod.prepare(queryInvCommodStr);
                for(int i = 0; i < commodities.size(); ++i)
                {
                    queryInvCommod.bindValue(":invoice_id", id_invoice);
                    queryInvCommod.bindValue(":commodity_id", commodities.at(i).id.toLongLong());
                    queryInvCommod.bindValue(":net", commodities.at(i).net.toDouble());
                    queryInvCommod.bindValue(":quantity", commodities.at(i).quantity.toInt());
                    queryInvCommod.bindValue(":discount", commodities.at(i).discount.toInt());
                    if(!queryInvCommod.exec())
                    {
                        qDebug() << "queryInvCommodStr: " << queryInvCommod.lastQuery();
                        QMapIterator<QString, QVariant> boundIter(queryInvCommod.boundValues());
                        while(boundIter.hasNext())
                        {
                            boundIter.next();
                            qDebug() << boundIter.key() << ": " << boundIter.value();
                        }

                        qDebug() << "lastError: " << queryInvCommod.lastError().text();
                        qDebug() << "INSERT error in 'table_invoice_commodity' table.\n";
                    }
                }
                if(db_.commit())
                {
                    modelInvoice_->select();
                }
                else
                {
                    errorMsg  = "Couldn't commit all changes in the DB.";
                }
            }
            else
            {
                qDebug() << "queryInvStr: " << queryInvStr;
                errorMsg = "INSERT error in 'invoice' table.";
            }
        }
        else
        {
            errorMsg = "Can't create a DB transaction.";
        }
    }

    if(!errorMsg.isEmpty())
    {
        qDebug() << "Database::invoiceWithCommoditiesInsertTransact(): " << errorMsg;
        db_.rollback();
        return false;
    }
    return true;
}


bool Database::invoiceWithCommoditiesDeleteTransact(const qint64 &id_invoice)
{
    QString errorMsg;

    if(db_.transaction())
    {
        QSqlQuery queryInv(modelInvoice()->query());

        const QString queryInvDel(QString("DELETE FROM `invoice` WHERE `id_invoice` = %1")
                                  .arg(id_invoice));
        if(queryInv.exec(queryInvDel))
        {
            if(db_.commit())
            {
                modelInvoice_->select();
            }
            else
            {
                errorMsg  = "Couldn't commit all changes in the DB.";
            }
        }
        else
        {
            errorMsg = "INSERT error in 'invoice' table.";
        }
    }
    else
    {
        errorMsg = "Can't create a DB transaction.";
    }

    if(!errorMsg.isEmpty())
    {
        qDebug() << "Database::submitInvoice(): " << errorMsg;
        db_.rollback();
        return false;
    }
    return true;
}


QList<CommodityVisualData> Database::commodities(const qint64 id_invoice)
{
    QList<CommodityVisualData> ret;
    QSqlQuery query(modelInvoiceWithCommodities_->query());
    const QString queryStr(QString("SELECT commodity.id_commodity, commodity.name, table_invoice_commodity.quantity, unit.name, commodity.pkwiu,table_invoice_commodity.net, commodity.vat, commodity_type.type, table_invoice_commodity.discount "
                                   "FROM commodity INNER JOIN table_invoice_commodity ON commodity.id_commodity = table_invoice_commodity.commodity_id "
                                   "INNER JOIN unit ON unit.id_unit = commodity.unit_id "
                                   "INNER JOIN commodity_type ON commodity_type.id_commodity_type = commodity.type_id "
                                   "WHERE table_invoice_commodity.invoice_id = %1").arg(id_invoice));
    query.exec(queryStr);
    if(query.isActive())
    {
        CommodityVisualData d;
        while(query.next())
        {
            for(int i = CommodityVisualFields::ID; i <= CommodityVisualFields::DISCOUNT; ++i)
            {
                d.setField(i, query.value(i));
            }
            ret.append(d);
        }
    }
    else
    {
        qDebug() << "Database::commodities() lastError: " << query.lastError().text();
    }
    return ret;
}
