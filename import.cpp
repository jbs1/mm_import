#include <iostream>
#include <QFile>
#include <QCoreApplication>
#include <string>
#include <QProcess>
#include <QString>
#include <QDebug>
using namespace std;


void newlist(QString ln, QString mail, QString pw){
    QProcess p_newlist;
    p_newlist.start("./bin/newlist", QStringList() << "-q" << ln << mail << pw);

    QFile alias_file("/etc/aliases");
    alias_file.open(QIODevice::WriteOnly | QIODevice::Text);

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
").arg(ln);

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




int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

//    cout << argc << argv[0] << endl;
    if(argc<=1 || strcmp(argv[1],"-h"))
    {
        cout << "This program allows you to import lists to mailman." << endl\
             << "Use it like this:" << endl\
             << "import {listname|filename importlist} {owner-email} {list-password}" << endl;
    } else {

        newlist(argv[1],argv[2],argv[3]);

    //./bin/config_list -i {path_to_cfg} {listname}
    //    run ./bin/withlist -l -r fix_url {listname}
    //    run ./bin/add_members -r {path_member_file} -w n -a n {listname}

        QProcess p_cflist;
        p_cflist.start("./bin/config_list", QStringList() << "-i" << QString(argv[1]) << QString(argv[1]));
        p_cflist.waitForFinished();

        QProcess p_fixurl;
        p_fixurl.start("./bin/withlist", QStringList() << "-l" << "-r" << "fix_url" << QString(argv[1]));
        p_fixurl.waitForFinished();


        QProcess p_addmem;
        p_addmem.start("./bin/add_members", QStringList() << "-r" << QString("%1.subscribers").arg(argv[1]) << "-w" << "n" << "-a" << "n" << QString(argv[1]));
        p_addmem.waitForFinished();


    }



    return 0;
//    return a.exec();
}
