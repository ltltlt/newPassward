/*************************************************************************
	> File Name: passward.cpp
	> Author: ty-l
	> Mail: liuty196888@gmail.com 
	> Created Time: 2016年03月17日 星期四 16时25分30秒
 ************************************************************************/

#ifndef __LTY_PASSWARD_2
#define __LTY_PASSWARD_2
#include<iostream>
#include<string>
#include<algorithm>
#include<map>
#include<fstream>
namespace lty{
using namespace std;
	class handler{
	public:
		typedef map<string,pair<string,string>> mymap;
		class notFound{	};		//useful for search
		class bad_key{	};
		handler(string key=""):__key(key){
			if(key=="")
				throw bad_key();
			fs.open("/home/ty-l/lty/passward2.0",ios_base::in);
			if(fs.is_open()){
				string name,account,passward;
				while(fs>>name>>account>>passward){
					__storer.insert(make_pair(name,make_pair(account,passward)));
				}
				fs.close();
			}
		}
		bool insert(string name,string account,string passward){
			int index=0;
			for_each(account.begin(),account.end(),[this,&index](char& ch){
					ch+=__key[index++];
					index%=__key.size();
					});
			index=0;
			for_each(passward.begin(),passward.end(),[this,&index](char& ch){
					ch+=__key[index++];
					index%=__key.size();
					});
			return __storer.insert(make_pair(name,make_pair(account,passward))).second;
		}
		bool erase(string name){
			mymap::iterator it=__storer.find(name);
			if(it!=__storer.end()){
				__storer.erase(it);
				return true;
			}
			return false;
		}
		const pair<string,string>& search(string name){
			static pair<string,string> temp;
			mymap::iterator it=__storer.find(name);
			if(it==__storer.end()){
				cerr<<"Cannot find this information!!"<<endl;
				throw notFound();
			}
			temp=it->second;
			int index=0;
			for_each(temp.first.begin(),temp.first.end(),[this,&index](char& ch){
					ch-=__key[index++];				//要考虑key是空字符的情况
					index%=__key.size();
					});
			index=0;
			for_each(temp.second.begin(),temp.second.end(),[this,&index](char& ch){
					ch-=__key[index++];
					index%=__key.size();
					});
			return temp;
		}
		bool change(string name,string account,string passward){
			mymap::iterator it=__storer.find(name);
			if(it!=__storer.end()){
				__storer.erase(it);
				insert(name,account,passward);
				return true;
			}
			return false;
		}
		~handler(){
			fs.open("/home/ty-l/lty/passward2.0",ios_base::out);
			for(pair<string,pair<string,string>> it:__storer)
				fs<<it.first<<" "<<it.second.first<<" "<<it.second.second<<endl;
			fs.close();
		}
	private:
		fstream fs;
		mymap __storer;
		string __key;			//加密密匙
	};
	void userface(){
		cout<<"key: ";
		string key;
		getline(cin,key,'\n');
		if(key==""){
			cerr<<"wtf"<<endl;
			return;
		}
		handler h(key);
		while(true){
			cout<<"s(store information)	f(find information)	c(change information)	d(delete information)	q(quit)"<<endl;
			char c;
			cin>>c;
			cin.get();			//get white space
			if(c=='s' or c=='c'){
				string name,passward,account;
				cout<<"name: ";
				getline(cin,name,'\n');
				cout<<"account: ";
				getline(cin,account,'\n');
				cout<<"passward: ";
				getline(cin,passward,'\n');
				if(c=='s' and h.insert(name,account,passward))
					cout<<"Success."<<endl;
				else if(c=='s')
					cerr<<"Already have this information"<<endl;
				else if(h.change(name,account,passward))
					cout<<"Success."<<endl;
				else
					cerr<<"Cannot find this information!!"<<endl;
			}else if(c=='f'){
				string name;
				cout<<"name: ";
				getline(cin,name,'\n');
				try{
					const pair<string,string>& temp=h.search(name);
					cout<<"account: "<<temp.first<<endl;
					cout<<"passward: "<<temp.second<<endl;
				}catch(handler::notFound&){
					cerr<<"Cannot find this information"<<endl;
				}
			}else if(c=='d'){
				string name;
				cout<<"name: ";
				getline(cin,name,'\n');
				if(h.erase(name))
					cout<<"Success."<<endl;
				else
					cerr<<"Cannot find this information!!"<<endl;
			}else if(c=='q')
				return;
			else{
				cerr<<"bad message"<<endl;
			}
		}
	}
};
#endif
