#ifndef APIKEYVALUEMGR_H
#define APIKEYVALUEMGR_H
#include "MySQLite.h"
class APIKeyValueMgr : public MySQLite
{
public:
    APIKeyValueMgr(const QString &dbpath,
                   const QString &connName,
                   const QString &DBName);
    ~APIKeyValueMgr();

protected:
    bool createTable();

public:
    bool addNewRecord(const QString &key,
                      const QString &keyname,
                      const bool &isUsed,
                      const int &useCount);

    bool isExist(const QString &key);
    bool isExistByName(const QString &name);

    bool updateUseDate(const QString &usedate, const QString &keyname);
    QVariant getUseDate(const QString &name);


    bool updateUseState(const QString &key,
                        const bool &isUsed);
    bool getUseState(const QString &key);

    bool updateUseCount(const QString &key,
                        const int &useCount);
    int getUseCount(const QString &key);

    QStringList getKeyName();
    QString getKeyValue(const QString &keyname);

    bool removeKey(const QString &key);

    QSqlDatabase *getdb();
    int getRowCount();
    QSqlTableModel *wordListDBModel(QWidget *parent,
                                    const QString &tableName);

private:
    QSqlTableModel *DBModel;
};
#endif // APIKEYVALUEMGR_H
