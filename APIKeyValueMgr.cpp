#include "APIKeyValueMgr.h"
#include <QDate>
/////////////////////////////////////////////
//

APIKeyValueMgr::APIKeyValueMgr(const QString &dbpath,
                               const QString &connName,
                               const QString &DBName)
            : MySQLite(dbpath, connName, DBName)
{
    if (!isTableExist("KeyList"))	// 新建的数据库,创建表
    {
        createTable();
    }
    DBModel = NULL;
}

APIKeyValueMgr::~APIKeyValueMgr()
{
    if(DBModel != NULL)
    {
        qDebug() << "移除DBModel";
        DBModel->deleteLater();
        DBModel = NULL;
    }
    qDebug() << "移除APIKeyValueMgr";
}

bool APIKeyValueMgr::createTable()
{
    db->transaction();
    QSqlQuery query(*db);
    query.exec("CREATE TABLE [KeyList] (\
               [Id] INTEGER NOT NULL ON CONFLICT FAIL PRIMARY KEY AUTOINCREMENT, \
               [Key] TEXT, \
               [KeyName] TEXT, \
               [lastUseTime] DATETIME NOT NULL ON CONFLICT FAIL,\
               [IsUsed] BOOL NOT NULL ON CONFLICT FAIL DEFAULT 0,\
               [UseCount] INTEGER);");

    db->commit();
    return true;
}

bool APIKeyValueMgr::addNewRecord(const QString &key,
                                  const QString &keyname,
                                  const bool &isUsed,
                                  const int &useCount)
{
    db->transaction();
    QString strCurrentDate = QDate::currentDate().toString("yyyy-MM-dd");
    QString sql = QString("insert into KeyList "
                          "(Key, KeyName, lastUseTime, IsUsed, UseCount)"
                          " values ('%1', '%2', '%3', '%4', '%5')")
            .arg(key)
            .arg(keyname)
            .arg(strCurrentDate)
            .arg(isUsed)
            .arg(useCount);
    exec(sql);
    db->commit();
    return true;
}

bool APIKeyValueMgr::isExist(const QString &key)
{
    QSqlQuery query(QString("select Id from KeyList where Key = '%1'").arg(key), *db);
    return query.next();
}

bool APIKeyValueMgr::isExistByName(const QString &name)
{
    QSqlQuery query(QString("select Id from KeyList where KeyName = '%1'").arg(name), *db);
    return query.next();
}

bool APIKeyValueMgr::updateUseDate(const QString &usedate, const QString &keyname)
{
    QString sql = QString("update KeyList set lastUseTime = '%1' where KeyName = '%2'")
            .arg(usedate)
            .arg(keyname);
    return exec(sql);
}

QVariant APIKeyValueMgr::getUseDate(const QString &name)
{
    QSqlQuery query(QString("select lastUseTime from KeyList where KeyName = '%1'").arg(name), *db);
    query.next();
    return query.record().value(0);
}

bool APIKeyValueMgr::updateUseState(const QString &key,const bool &isUsed)
{
    QString sql = QString("update KeyList set IsUsed = '%1' where Key = '%2'")
            .arg(QString::number(isUsed))
            .arg(key);
    return exec(sql);
}

bool APIKeyValueMgr::getUseState(const QString &key)
{
    QSqlQuery query(QString("select IsUsed from KeyList where Key = '%1'").arg(key), *db);
    query.next();
    return query.record().value(0).toBool();
}

bool APIKeyValueMgr::updateUseCount(const QString &key, const int &useCount)
{
    QString sql = QString("update KeyList set UseCount = '%1' where Key = '%2'")
            .arg(useCount)
            .arg(key);
    return exec(sql);
}

int APIKeyValueMgr::getUseCount(const QString &key)
{
    QSqlQuery query(QString("select UseCount from KeyList where Key = '%1'").arg(key), *db);
    query.next();
    return query.record().value(0).toInt();
}

QStringList APIKeyValueMgr::getKeyName()
{
    QStringList keyName;
    QSqlQuery query("select KeyName from KeyList", *db);
    while (query.next())
    {
        keyName << query.record().value("KeyName").toString();
    }

    return keyName;
}

QString APIKeyValueMgr::getKeyValue(const QString &keyname)
{
    QSqlQuery query(QString("select Key from KeyList where KeyName = '%1'").arg(keyname), *db);
    query.next();
    return query.record().value(0).toString();
}

bool APIKeyValueMgr::removeKey(const QString &key)
{
    if(isExist(key))
    {
        QString sql = QString("delete from KeyList where Key = '%1'")
            .arg(key);
        return exec(sql);
    }
    else
    {
        return false;
    }
}

QSqlDatabase *APIKeyValueMgr::getdb()
{
    return db;
}

int APIKeyValueMgr::getRowCount()
{
    QSqlQuery query("select count(*) from KeyList", *db);
    query.next();
    return query.value(0).toInt();
}

QSqlTableModel *APIKeyValueMgr::wordListDBModel(QWidget *parent,
                                                const QString &tableName)
{
    DBModel = new QSqlTableModel(parent,*db);
    DBModel->setTable(tableName);
    DBModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    DBModel->select(); //选取整个表的所有行
    return DBModel;
}

