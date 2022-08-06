//#include "Process.h"
//#include <sys/stat.h>
#include<fstream>
#include<iostream>
#include<stdio.h>
#include<random>
#include<algorithm>
#include<vector>
//#include<bitset>
#include<map>
#include<set>
#include <stdio.h>
#include<unordered_map>
//#define DOC_NUM 1000;
using namespace std;

vector<string> split(string str,string pattern){
	vector<string> res;

	int pos = str.find(pattern);

	while(pos!=string::npos){
		string word = str.substr(0,pos);
		str = str.substr(pos+1);
		res.push_back(word);
		pos = str.find(pattern);
	}

	return res;
}

bool  cmp(pair<string,vector<string>>& a1, pair<string,vector<string>>& a2){
	return a1.second.size()>a2.second.size();
};

void ProdeceInvertFromForward2(string file){
	
	ifstream in(file);
	ofstream inverted_file100("/home/node11/ProcessData/HotWords/Inverted_Index100.txt");
	ofstream inverted_file500("/home/node11/ProcessData/HotWords/Inverted_Index500.txt");
	ofstream inverted_file1000("/home/node11/ProcessData/HotWords/Inverted_Index1000.txt");
	ofstream inverted_file10000("/home/node11/ProcessData/HotWords/Inverted_Index10000.txt");
	ofstream inverted_file25000("/home/node11/ProcessData/HotWords/Inverted_Index25000.txt");
	ofstream inverted_file50000("/home/node11/ProcessData/HotWords/Inverted_Index50000.txt");
	
	unordered_map<string,vector<string>> m;
	//vector<string> m;
	string line;
	
	if(!in)
	{
		cout<<"Open file failed"<<endl;
		return;
	}else if(in){

		while(getline(in,line)){
			int pos=line.find('\t');
			string id=line.substr(0,pos);
			
			line = line.substr(pos+1);
			vector<string> words = split(line," ");

			for(auto word:words){
				if(m.count(word)){
					m[word].push_back(id);
				}else{
					m.insert(pair<string,vector<string>> {word,{id}});
				}
			}
			
		}
	}
	vector<pair<string,vector<string>>> res(m.begin(),m.end());
	sort(res.begin(),res.end(),cmp);
	int k = 0;
	for(auto i : res){
		k++;
		inverted_file100<<i.first;
		inverted_file100<<endl;
		if(k == 100) break;
	}

	k = 0;
	for(auto i : res){
		k++;
		inverted_file500<<i.first;
		inverted_file500<<endl;
		if(k == 500) break;
	}

	k = 0;
	for(auto i : res){
		k++;
		inverted_file1000<<i.first;
		inverted_file1000<<endl;
		if(k == 1000) break;
	}


	k = 0;
	for(auto i : res){
		k++;
		inverted_file10000<<i.first;
		inverted_file10000<<endl;
		if(k == 10000) break;
	}


	k = 0;
	for(auto i : res){
		k++;
		inverted_file25000<<i.first;
		inverted_file25000<<endl;
		if(k == 25000) break;
	}

	k = 0;
	for(auto i : res){
		k++;
		inverted_file50000<<i.first;
		inverted_file50000<<endl;
		if(k == 50000) break;
	}




	in.close();
	inverted_file100.close();
	inverted_file500.close();
	inverted_file1000.close();
	inverted_file10000.close();
	inverted_file25000.close();
	inverted_file50000.close();

	return;
}


void ProduceForwardFromWset(int fileno){
	ofstream forward_file("/home/node11/ProcessData/forwardFiles/test_forward_Index.txt");
	for(int i = 0; i < fileno; i++){
		forward_file<<i<<'\t';//写入单词
		ifstream in("/home/node11/ProcessData/wordSet/"+ std::to_string(i));
		if(!in)
		{
		cout<<"Open file failed"<<endl;
		return;
		}
		string line;
		while(getline(in,line))
        {
          //string::size_type pos_key=line.find('\n');//无符号类型的值

          //if(pos_key!=string::npos)//如果存在 非npos
          //{
            //string key=line.substr(0,pos_key-1);
        	forward_file<<line;
          //}
		}
		forward_file<<'\n';
	}
	return;
}

void ProduceInvertFromHot(int num_IN,int num_Doc)
{
	//num_In需要的行数  begin_num Id的开始大小 num_Doc为id数量
	string file="/home/node11/ProcessForOXT/HotWords/Inverted_Index100.txt";
	ifstream in(file);
	default_random_engine e0;//创建引擎 未设置数字
	int seed=32767;
	default_random_engine e1(seed);
	default_random_engine e2(25369);
	uniform_int_distribution<unsigned> n(1,num_Doc);
	uniform_int_distribution<unsigned> n_key(1,num_IN);
	vector<int> key_Index;
	vector<string> vec_key;

	string line;
	int count=0;
	if(!in)
	{
		cout<<"Open file failed"<<endl;
	}

	//prodece key this mean search different line先将line打乱
	for(int i=0;i<num_IN;i++)
	{
	int num=n_key(e2);//0~25369
	vector<int>::iterator it=find(key_Index.begin(),key_Index.end(),num);
	if(it!=key_Index.end())
	{
		i--;
		continue;
	}
	key_Index.push_back(num);
	while(getline(in,line))
	{
		count++;
		if(count==num)
		{
			vec_key.push_back(line);
			count=0;
			in.seekg(0,ifstream::beg);
			break;
		}
	}
	}

	//produce ID 生产id
	vector<int> ch;

	for(int i=1;i<=num_Doc;i++)
	{
		ch.push_back(i);
	}


	string fileName="/home/node11/ProcessForOXT/invertwords/Inverted_Index.txt";
	ofstream file_out(fileName);


	int count_test=0;
	int max_ID=0;

	for(size_t i=0;i<num_IN;i++)
	{
		if(file_out)
		{
		shuffle(ch.begin(),ch.end(),e1);//打乱数组元素
		
		int m=n(e0);//m为生成id的数目
		if(m>num_Doc/2)
		{
			i--;
			continue;
		}
		file_out<<vec_key.at(i)<<'\t';//输出单词
		for(int j=0;j<m;j++)
		{
			if(j<num_Doc&&i<num_IN)
			{
			file_out<<ch.at(j)<<' ';//输出随机id
			}
		}
		file_out<<'\n';
		e1.seed(seed+1);
		}
	}
	file_out.close();
}

int main()
{
    ProduceInvertFromHot(100,100);
    return 0;
}