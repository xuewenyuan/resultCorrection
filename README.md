# resultCorrection
Belong to LaboratorySheetOCR  
该程序是resultCorrection函数的测试程序，resultCorrection函数负责对LaboratorySheetOCR工程识别后的结果进行校对。  
  
// 对前阶段识别结果进行校正  
// 输入：识别结果 类型 vector \<string\>  
//      项目1  
//      结果1  
//      项目2  
//      结果2  
//      ...  
// 输出：校正后的结果 类型 string  
//      项目1 结果1 项目2 结果2 ....  
// 返回值：0代表正常  
int resultCorrection(string & rusultAFCorrection, vector<string> resultAFRec)
