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


void newlist(QString ln, QString mail, QString pw, QString mmpath){
    QProcess p_newlist;
    p_newlist.start(QString("%1/bin/newlist").arg(mmpath), QStringList() << "-q" << ln << mail << pw);
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

void archive(QString listpath, QString mailmanpath){
    QFile arc(QString("%1.mbox").arg(listpath));
    if(arc.exists()){
        QProcess p_ar;
        p_ar.start(QString("%1/bin/arch").arg(mailmanpath), QStringList() << get_listname(listpath) << QString("%1.mbox/%2.mbox").arg(listpath,get_listname(listpath)));
        p_ar.waitForFinished();
    }
}



void singleimport(QString listpath, QString email, QString pw, QString mmdir){

    newlist(get_listname(listpath),email,pw,mmdir);
    cout<< "LIST CREATED: " << get_listname(listpath).toStdString() << endl;

    QProcess p_cflist;
    p_cflist.start(QString("%1/bin/config_list").arg(mmdir), QStringList() << "-i" << QString(listpath) << get_listname(listpath));
    p_cflist.waitForFinished();
    cout<< "LIST CONFIGURED: " << get_listname(listpath).toStdString() << endl;

    QProcess p_fixurl;
    p_fixurl.start(QString("%1/bin/withlist").arg(mmdir), QStringList() << "-l" << "-r" << "fix_url" << get_listname(listpath));
    p_fixurl.waitForFinished();
    cout<< "FIXED URL: " << get_listname(listpath).toStdString() << endl;


    QProcess p_addmem;
    p_addmem.start(QString("%1/bin/add_members").arg(mmdir), QStringList() << "-r" << QString("%1.subscribers").arg(listpath) << "-w" << "n" << "-a" << "n" << get_listname(listpath));
    p_addmem.waitForFinished();
    cout<< "ADDED MEMBERS: " << get_listname(listpath).toStdString() << endl;

    QProcess p_genal;
    p_genal.start("./genaliases");
    p_genal.waitForFinished();
    cout<< "EXEC GENAL: " << get_listname(listpath).toStdString() << endl;

    QProcess p_checkperm;
    p_checkperm.start(QString("%1/bin/check_perms").arg(mmdir), QStringList() << "-f");
    p_checkperm.waitForFinished();
    cout<< "FIXED PERMS: " << get_listname(listpath).toStdString() << endl;


    archive(listpath,mmdir);
    cout<< "ADDED ARCHIVES: " << get_listname(listpath).toStdString() << endl;
}



int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

    if(argc<=1)
    {
        cout << "This program allows you to import lists to mailman." << endl\
             << "Use it like this:" << endl\
             << "import <filename/path import> <owner-email> <list-password> <mailman-dir>" << endl;
    } else {
        singleimport(argv[1]),argv[2],argv[3],argv[4]);
    }


    return 0;
//    return a.exec();
}
