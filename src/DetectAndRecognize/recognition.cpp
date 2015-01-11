#include "recognition.hpp"


facerecognizer::facerecognizer ( std::string const& test )
{
}

facerecognizer::~facerecognizer ()
{

}

void facerecognizer::recognize_face( cv::Mat const& face, cv::Mat const& comp_face )
{

}

bool facerecognizer::openDB()
    {
    //find QSLite driver
    db = QSqlDatabase::addDatabase("QSQLITE");

    #ifdef Q_OS_LINUX
    //store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("my.db.sqlite");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
    #else
    //file exists in the application private folder,
    db.setDatabaseName("my.db.sqlite");
    #endif

    //open database
    return db.open();
    }

bool facerecognizer::deleteDB()
    {
    //close database
    db.close();

    #ifdef Q_OS_LINUX
    //we have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("my.db.sqlite");
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
    #else

    //remove created database binary file
    return QFile::remove("my.db.sqlite");
    #endif
    }

QSqlError facerecognizer::lastError()
    {
    //if opening the database has failed, user can ask error description by QSqlError::text()
    return db.lastError();
    }

int facerecognizer::insertModel(QString firstname, QString lastname, QByteArray mask)
    {
    int newId = -1;
    bool ret = false;

    if (db.isOpen())
        {

        // NULL = is the keyword for the autoincrement to generate next value
        QSqlQuery query;
        ret = query.exec(QString("insert into person values(NULL,'%1','%2',%3)")    //Anfrage Datenbank
        .arg(firstname).arg(lastname));

        //get database given autoincrement value
        if (ret)
            {
            newId = query.lastInsertId().toInt();
            }

        }
    return newId;
    }

bool facerecognizer::createModelTable()
    {
    //create table "model"
    bool ret = false;
    if (db.isOpen())
        {
        QSqlQuery query;
        ret = query.exec("create table model "
                  "(id integer primary key, "
                  "firstname varchar(30), "
                  "lastname varchar(30), "
                  "mask Mat)");

        }
    return ret;
    }

bool facerecognizer::getModel(int id, RecognitionModel*& model)
    {
    bool ret = false;

    QSqlQuery query(QString("select * from model where id = %1").arg(id));
    if (query.next())
        {
        model->id = query.value(0).toInt();
        model->firstname = query.value(1).toString();
        model->lastname = query.value(2).toString();
        model->mask = query.value(3).toByteArray();
        ret = true;
        }

    return ret;
    }

