#include <iostream>
#include <QFile>
#include <QCoreApplication>
#include <string>
#include <QProcess>
#include <QString>
#include <QDebug>
#include <QFileInfo>
using namespace std;


QString get_listname(QString path){
    QFileInfo file(path);
    QString name=file.fileName();
    return name;
}


void newlist(QString ln, QString mail, QString pw){
    QProcess p_newlist;
    p_newlist.start("./bin/newlist", QStringList() << "-q" << ln << mail << pw);
    p_newlist.waitForFinished();

    QFile alias_file("/etc/aliases");
    alias_file.open(QIODevice::WriteOnly | QIODevice::Append);

    if(alias_file.isOpen()){
QString al=QString("%1:              \"|/var/lib/mailman/mail/mailman post %1\"\n\
%1-admin:        \"|/var/lib/mailman/mail/mailman admin %1\"\n\
%1-bounces:      \"|/var/lib/mailman/mail/mailman bounces %1\"\n\
%1-confirm:      \"|/var/lib/mailman/mail/mailman confirm %1\"\n\
%1-join:         \"|/var/lib/mailman/mail/mailman join %1\"\n\
%1-leave:        \"|/var/lib/mailman/mail/mailman leave %1\"\n\
%1-owner:        \"|/var/lib/mailman/mail/mailman owner %1\"\n\
%1-request:      \"|/var/lib/mailman/mail/mailman request %1\"\n\
%1-subscribe:    \"|/var/lib/mailman/mail/mailman subscribe %1\"\n\
%1-unsubscribe:  \"|/var/lib/mailman/mail/mailman unsubscribe %1\"\n\
\n").arg(ln);

        alias_file.write(al.toUtf8());
        alias_file.close();
    } else {
        qCritical()<<"Aliases file could not be opened!";
        exit(1);
    }

    QProcess p_newal;
    p_newal.start("newaliases");
    p_newal.waitForFinished();

    QProcess p_ps;
    p_ps.start("/etc/init.d/postfix", QStringList() << "restart");
    p_ps.waitForFinished();
}

void archive(QString listpath){
    QFile arc(QString("%1.mbox").arg(listpath));
    if(arc.exists()){
        QProcess p_ar;
        p_ar.start("./bin/arch", QStringList() << get_listname(listpath) << QString("%1.mbox/%2.mbox").arg(listpath,get_listname(listpath)));
        p_ar.waitForFinished();
    }
}



int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

    if(argc<=1)
    {
        cout << "This program allows you to import lists to mailman." << endl\
             << "Use it like this:" << endl\
             << "import {[path &]filname} {owner-email} {list-password}" << endl;
    } else {
        newlist(get_listname(argv[1]),argv[2],argv[3]);
        cout<< "LIST CREATED: " << get_listname(argv[1]).toStdString() << endl;

        QProcess p_cflist;
        p_cflist.start("./bin/config_list", QStringList() << "-i" << QString(argv[1]) << get_listname(argv[1]));
        p_cflist.waitForFinished();
        cout<< "LIST CONFIGURED: " << get_listname(argv[1]).toStdString() << endl;

        QProcess p_fixurl;
        p_fixurl.start("./bin/withlist", QStringList() << "-l" << "-r" << "fix_url" << get_listname(argv[1]));
        p_fixurl.waitForFinished();
        cout<< "FIXED URL: " << get_listname(argv[1]).toStdString() << endl;


        QProcess p_addmem;
        p_addmem.start("./bin/add_members", QStringList() << "-r" << QString("%1.subscribers").arg(argv[1]) << "-w" << "n" << "-a" << "n" << get_listname(argv[1]));
        p_addmem.waitForFinished();
        cout<< "ADDED MEMBERS: " << get_listname(argv[1]).toStdString() << endl;

//        QProcess p_genal;
//        p_genal.start("./genaliases");
//        p_genal.waitForFinished();

        QProcess p_checkperm;
        p_checkperm.start("./bin/check_perms", QStringList() << "-f");
        p_checkperm.waitForFinished();
        cout<< "FIXED PERMS: " << get_listname(argv[1]).toStdString() << endl;


        archive(argv[1]);
        cout<< "ADDED ARCHIVES: " << get_listname(argv[1]).toStdString() << endl;


    }


    return 0;
//    return a.exec();
}
