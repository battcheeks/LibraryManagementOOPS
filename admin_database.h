#include<iostream>
#include<fstream>
#include<vector>
#include<ctime>
#include"admin.h"
using namespace std;

class admin_database
{
    public:
        void upload(admin &ad)
        {
            ofstream ofile;
            ofile.open("admin.bin",ios::out|ios::app|ios::binary);
            ofile.write((char*)&ad,sizeof(ad));
            ofile.close();
            log("upload 1 admin");
        }
        vector<admin> download()
        {
            vector<admin> data;
            admin ad;
            ifstream ifile;
            ifile.open("admin.bin",ios::in|ios::binary);
            while(ifile.read((char*)&ad,sizeof(ad)))
            {
                data.push_back(ad);
            }
            ifile.close();
            log("downloaded all data");
            return data;
        }
        bool extract(vector<admin> &info)
        {
            if(info.size()==0){cout<<"No data"<<endl;}
            for(int i=0;i<info.size();i++)
            {
                info[i].display();
            }
            cout<<"Total Data : "<<info.size()<<endl;
            cout<<"Return to Menu (y/n) : ";
            char choice;
            cin>>choice;
            if(choice=='y'||choice=='Y')
                return true;
            else
                return false;
        }
        vector<admin> searchbyAdminID(char element[])
        {
            vector<admin> searchRes;
            cout<<"Key :"<<element<<endl; 
            admin ad;
            ifstream ifile;
            ifile.open("admin.bin",ios::in|ios::binary);
            while(ifile.read((char*)&ad,sizeof(ad)))
            {
                if(strcmp(ad.get_admin_id(),element)==0)
                {
                    searchRes.push_back(ad);
                }
            }
            ifile.close();
            log("searchbyadmin");
            return searchRes;
        }

        vector<admin> searchbyName(char *Name)
        {
            vector<admin> searchRes;
            cout<<"Key : "<<Name<<endl;
            admin ad;
            ifstream ifile;
            ifile.open("admin.bin",ios::in|ios::binary);
            while(ifile.read((char*)&ad,sizeof(ad)))
            {
                if(strcmp(ad.get_name(),Name)==0)
                {
                    searchRes.push_back(ad);
                }
            }
            ifile.close();
            log("searchbyname");
            return searchRes;
            
        }
        bool login(char* uname, char* pass)
        {
            cout<<uname<<" "<<pass<<endl;
            admin ad;
            char mn[100];
            strcpy(mn,"MASTER@ADMIN");
            char mp[100];
            strcpy(mp,"ADMIN123");
            ifstream ifile;
            ifile.open("admin.bin",ios::in|ios::binary);
            while(ifile.read((char*)&ad,sizeof(ad)))
            {
                if(((strcmp(ad.get_username(),uname)==0) && (strcmp(ad.get_password(),pass)==0)) || ((strcmp(uname,mn)==0)&& (strcmp(pass,mp)==0)))
                {
                    return true;
                }
            }
            ifile.close();
            return false;
        }
        bool masterLogin()
        {
            char uname[100], pass[100];
            cout<<"Username : ";
            cin.ignore();
            cin.getline(uname,100);
            cout<<"Password : ";
            cin.getline(pass,100);
            char user[100], password[100];
            strcpy(user, "MASTER@ADMIN");
            strcpy(password,"ADMIN123");

            if(strcmp(uname,user)==0 && strcmp(pass, password)==0)
            {
                cout<<"WELCOME MASTER ADMIN "<<endl<<endl;
                log("master logged in");
                return true;
            }
            cout<<"This is Restricted Access!!"<<endl;
            log("master access restricted");
            return false;
        }
        void log(string work)
        {
            ofstream ofile;
            ofile.open("log.txt",ios::out|ios::app);
            time_t now = time(0);
            string dt = ctime(&now);
            dt= dt+ " ::::::: "+ work;
            ofile<<dt<<endl;
            ofile.close();
        }
        void open_log()
        {
            if(!masterLogin()){cout<<"Restricted Access !! "<<endl; return;}
            string topicName = "notepad \"log.txt\"";
            system(topicName.c_str());
        }
        bool deleteRecord(char element[])
        {
            admin  ad;
            cout<<"key :"<<element<<endl;
            int found=0;
            char choice;
            ofstream ofile("temp.bin",ios::out|ios::app|ios::binary);
            ifstream ifile;
			ifile.open("admin.bin",ios::in|ios::binary);
            
            while(ifile.read((char*)&ad,sizeof(ad)))
            {
                if(strcmp(ad.get_admin_id(),element)==0)
                {
                    ad.display();
                    found=1;
                    cout<<"Do you want to delete ? (y/n) : ";
                    cin>>choice;
                    if(choice=='n'||choice=='N')
                    {
                        ofile.write((char*)&ad,sizeof(ad));
                    }
                }
                else
                {
                    ofile.write((char*)&ad,sizeof(ad));
                }
            }
            ifile.close();
            ofile.close();

            remove("admin.bin");
            rename("temp.bin","admin.bin");
            ifile.open("admin.bin",ios::in);
            if(!ifile){cout<<"Database got Corrupted !!"; return false;}
            ifile.close();
            if(found==0)
            {
                cout<<"Record not Found !! ";
                log("record admin id not found");
                return false;
            }
            if(choice=='y'||choice=='Y')
            {
                cout<<"Deleted succefully"<<endl;
                log("record admin id  deleted successfull");
            }
            else
            {
                cout<<"Delete Aborted!!!"<<endl;
                log("record admin id deleted aborted");
            }
            
            return true;
        }
};
