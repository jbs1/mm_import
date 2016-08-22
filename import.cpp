#include <iostream>
#include <QFile>
#include <QCoreApplication>
#include <string>
#include <QProcess>
#include <QString>
using namespace std;



//void newlist(){
//    QFile set_file;
//    set_file.setFileName(setjson);
//    set_file.open(QIODevice::WriteOnly | QIODevice::Text);
//    if(set_file.isOpen()){
//        QJsonDocument set_doc(set_ob);
//        set_file.write(set_doc.toJson());
//        set_file.close();
//    } else {
//        qCritical()<<"Settings file could not be opened!";
//    }
//}


//./newlist -q {listname} {owner-email} {list-password}

void newlist(QString ln, QString mail, QString pw){
    QProcess process;
    process.start("./newlist", QStringList() << "-q" << ln << mail << pw);
    QString out(process.readAllStandardOutput());
    cout << out.toStdString() << "sadsaasd" << endl;
}




int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

//    cout << argc << argv[0] << endl;
    if(argc<=1 || strcmp(argv[1],"-h"))
    {
        cout << "This program allows you to import lists to mailman." << endl\
             << "Use it like this:" << endl\
             << "import {listname|filename importlist} {owner-email} {list-password}" << endl;
    }

    newlist("test","dsads","sdasdas");








    return 0;
//    return a.exec();
}
