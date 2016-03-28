//
//  main.cpp
//  resultCorrection
//
//  Created by Jacob on 16/3/23.
//  Copyright © 2016年 Jacob. All rights reserved.
//

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define _UTF
//#define _Unicode


// 对前阶段识别结果进行校正
// 输入：识别结果 类型 vector<string>
//      项目1
//      结果1
//      项目2
//      结果2
//      ....
// 输出：校正后的结果 类型 string
//      项目1 结果1 项目2 结果2 ....
// 返回值：0代表正常
int resultCorrection(string & rusultAFCorrection, vector<string> resultAFRec){

	if (resultAFRec.empty()) {
		printf("Error in resultCorrection Function: 传入的识别字符为空\n");
		return -1;
	}

	struct char_statistics {
		int number;
		string words;
		map<string, int> char_num;
	}_char_statistics[1000];

	//将文件读入结构体数组
	FileStorage fs("char_num.xml", FileStorage::READ);
	FileNode features = fs["chars_statistics"];
	FileNodeIterator fni = features.begin();
	FileNodeIterator fniEnd = features.end();
	int len = (int)fs["frameCount"];//词条的个数
	if (len < 1) {
		printf("Error in resultCorrection Function: 字符表为空\n");
		return -1;
	}
	for (; fni!=fniEnd && len<=1000; fni++, len ++) {
		_char_statistics[len].number = (int)(*fni)["number"];
		_char_statistics[len].words = (string)(*fni)["words"];
		int char_count = (int)(*fni)["count"];
		for (int i = 1; i <= char_count; i ++) {
			stringstream _char,_num;
			_char << "char" << i;
			_num << "num" << i;
			_char_statistics[len].char_num[(string)(*fni)[_char.str()]] = (int)(*fni)[_num.str()];
		}

	}

	for (int i = 0; i < (int)resultAFRec.size(); i ++) {
		//统计每个已识别字条中的字符数量
		map<string, int> char_resultAFRec;
		for (size_t  j = 0; j < resultAFRec[i].size(); j++ ) {
			string temp;
#ifdef _UTF//UTF_8中文占3个字节
			if (resultAFRec[i][j] < 0) {
				temp = resultAFRec[i].substr(j,3);
				j += 2;
			}
			else{
				temp = resultAFRec[i].substr(j,1);
			}
#endif
#ifdef _Unicode//Unicode中文占2个字节
			if (resultAFRec[i][j] > 0 && resultAFRec[i][j] < 255) {
				temp = resultAFRec[i].substr(j,1);
			}
			else {
				temp = resultAFRec[i].substr(j,2);
				j += 1;
			}
#endif
			//cout << temp << " ";
			char_resultAFRec[temp] += 1;
 		}
		//cout << endl;
		//求相似度
		int maSum = 0;
		//设置初始值为原识别字符串
		string maString = resultAFRec[i];
		for (int j = 1; j <= len; j ++){
			map<string,int>::iterator ite;
			int sum = 0;
			for (ite = char_resultAFRec.begin(); ite != char_resultAFRec.end(); ite ++) {
				string key = ite->first;
				_char_statistics[j].char_num[key] += 0;
				sum += char_resultAFRec[key] * _char_statistics[j].char_num[key];
			}
			if (sum > maSum) {
				maSum = sum;
				maString = _char_statistics[j].words;
			}
		}
		rusultAFCorrection += maString;
		rusultAFCorrection += " ";
	}

	return 0;
}



int main(int argc, const char * argv[]) {


	vector<string> reAFrc;

	reAFrc.push_back("乙肝面抗原");
	reAFrc.push_back("乙肝面抗本");
	reAFrc.push_back("乙肝E抗原");
	reAFrc.push_back("乙肝E抗体");
	reAFrc.push_back("乙肝核心抗体");
	reAFrc.push_back("日性");
	reAFrc.push_back("月生");
	/*
	string temp = "乙i肝面抗原";
	for (size_t i = 0; i < temp.size(); ) {
		if (temp[i] < 0) {
			cout << temp.substr(i,3) << " ";
			i += 3;
		}
		else {
			cout << temp.substr(i,1) << " ";
			i += 1;
		}
	}
	*/
	cout << "应当输出： \n";
	cout << "乙肝表面抗原 乙肝表面抗体 乙肝E抗原 乙肝E抗体 乙肝核心抗体 阳性 阴性\n";
	cout << "实际输出： \n";
	string output = "";
	if (resultCorrection(output,reAFrc)==0) {
		cout << output << endl;
	}
	else cout << "err" << endl;

    return 0;
}
