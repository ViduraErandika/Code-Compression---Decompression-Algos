#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std; 

string XORconvert(string x, string y)
{
    string result;
    for (int i = 0; i < x.length(); i++)
    {
        if (x[i] == y[i])
            result += "0";
        else
            result += "1";
    }
    return result;
}
//calculate one's in a string
int countOnes(string s)
{
    int count = 0;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '1')
        {
            count++;
        }
    }
    return count;
}

//binary to decimal
int binaryToDecimal(int n)
{
    int num = n;
    int dec_value = 0;
    int base = 1;
    int temp = num;
    while (temp)
    {
        int last_digit = temp % 10;
        temp = temp / 10;
        dec_value += last_digit * base;
        base = base * 2;
    }
    return dec_value;
}
//decimal to binary
string decimalToBinary(int n)
{
    int binaryNum[32];
    string binary;
    int i = 0;
    while (n > 0)
    {
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
    for (int j = i - 1; j >= 0; j--)
    {
        binary = binary + to_string(binaryNum[j]);
    }
    return binary;
}

/* Decompression Begin*/

void genBitsAndDict(string compressFileName,string &compressBits, map<string, string> &compressDict){
   fstream file;
   vector<string> comDictGen;
   bool change = false;

   file.open(compressFileName); 
   if (file.is_open()){ 
      string line;
      while(getline(file, line)){ 
        if(!change && line != "xxxx"){
            compressBits=compressBits+line;
        }
        else if(change){
            comDictGen.push_back(line);
        }
        if (line=="xxxx"){
         change = true;
        }
      } }
   
   file.close(); 

   vector<string> dictIndex={"000","001","010","011","100","101","110","111"};
    for(int i=0;i<comDictGen.size();i++){
         compressDict[dictIndex[i]]=comDictGen[i];
    }
}

void genOriginal(string &compressBits, map<string, string> &compressDict, vector<string> &decompressedIns){
    int step = 0;
    for(int i=0;i<compressBits.size();i= i + step){
        string index = compressBits.substr(i,3);
        if (index == "000"){
            step = 5;
            string ins = compressBits.substr(i+3,2);
            int occurence = binaryToDecimal(stoi(ins));
            for (int j=0;j<occurence+1;j++){
                decompressedIns.push_back(decompressedIns.back());
            }
        }
        else if(index == "001"){
            step = 15;
            int position = binaryToDecimal(stoi(compressBits.substr(i+3,5)));
            int bitMask = stoi(compressBits.substr(i+8,4));
            string dictIndex = compressBits.substr(i+12,3);
            string ins = compressDict[dictIndex];
            string newBits = decimalToBinary(binaryToDecimal(stoi(ins.substr(position,4)))^binaryToDecimal(bitMask));
            string zeroPaded = string(4-newBits.size(),'0')+newBits;
            
            string newIns = ins.substr(0,position)+zeroPaded+ ins.substr(position + 4,ins.size()-(position+4));
            decompressedIns.push_back(newIns);            
        }
        else if(index == "010"){
            step = 11;
            int position = binaryToDecimal(stoi(compressBits.substr(i+3,5)));
            string dictIndex = compressBits.substr(i+8,3);
            string ins = compressDict[dictIndex];
            if (ins[position] == '1'){
                ins[position] = '0';
            }
            else{
                ins[position] = '1';
            }
            decompressedIns.push_back(ins);
        }
        else if(index == "011"){
            step = 11;
            int position = binaryToDecimal(stoi(compressBits.substr(i+3,5)));
            int secPosition = position+1;
            string dictIndex = compressBits.substr(i+8,3);
            string ins = compressDict[dictIndex];
            if (ins[position] == '1'){
                ins[position] = '0';
            }
            else{
                ins[position] = '1';
            }
            if (ins[secPosition] == '1'){
                ins[secPosition] = '0';
            }
            else{
                ins[secPosition] = '1';
            }
            decompressedIns.push_back(ins);
        }
        else if(index == "100"){
            step = 16;
            int position = binaryToDecimal(stoi(compressBits.substr(i+3,5)));
            int secPosition = binaryToDecimal(stoi(compressBits.substr(i+8,5)));
            string dictIndex = compressBits.substr(i+13,3);
            string ins = compressDict[dictIndex];
            if (ins[position] == '1'){
                ins[position] = '0';
            }
            else{
                ins[position] = '1';
            }
            if (ins[secPosition] == '1'){
                ins[secPosition] = '0';
            }
            else{
                ins[secPosition] = '1';
            }
            decompressedIns.push_back(ins);
        }
        else if(index == "101"){
            step = 6;
            string dictIndex = compressBits.substr(i+3,3);
            decompressedIns.push_back(compressDict[dictIndex]);
        }
        else if(index == "110"){
            step = 35;
            string ins = compressBits.substr(i+3,32);
            decompressedIns.push_back(ins);
        }
        else if(index == "111"){
            break;
        }
    }
}
/* Decompression End*/


/* Compression Begin*/

//store strings in file to array
void storeFile(string originalFileName, vector<string> &originalBits)
{
    ifstream file;
    file.open(originalFileName);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            originalBits.push_back(line);
        }
        file.close();
    }
}

void frequencyCounter(vector<string> originalBits,map<string, string> &originalDict)
{
    vector <string> vectorSet;
    vector <string> oriDictGen;

    for (int i = 0; i < originalBits.size(); i++)
        {
            string val = originalBits[i];
            if(find(vectorSet.begin(), vectorSet.end(), val) == vectorSet.end() ) { vectorSet.push_back(val); };
        }
    int max = 0;
    string maxString;
    int position;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < vectorSet.size(); j++)
        {
            string val = vectorSet[j];
            int freq = count(originalBits.begin(), originalBits.end(), val);

            if (freq > max)
            {
                max = freq;
                maxString = val;
                position = j;
            }else if(freq == max){
                continue;
            }
        }
        max = 0;
        vectorSet.erase(vectorSet.begin() + position);
        oriDictGen.push_back(maxString);
    }
    vector<string> dictIndex={"000","001","010","011","100","101","110","111"};
    for(int i=0;i<oriDictGen.size();i++){
         originalDict[dictIndex[i]]=oriDictGen[i];
    }
    }

void directMap(map<string,string> &dict, vector<string> &originalBits,vector<string> &compressedbit){   
    for(int i=0;i<originalBits.size();i++){
        string ins = originalBits[i];
        bool found = false;
        for (auto const& x : dict)
        {
            if (x.second == ins)
            {
                compressedbit.push_back("101"+x.first);
                found = true;
            }
        }
        if (!found){
            compressedbit.push_back(ins);
        }
    }
}

void runLengthEncode(vector<string> &compressedbit){
    int step = 1;
    string previousVal;
    string currentVal;
    for (int i = 0; i < compressedbit.size(); i = i + step)
    {
       int count = 0;
       step = 1;
       if(i>0){
        previousVal = compressedbit[i-1];
       }
       currentVal = compressedbit[i];
         if(previousVal == currentVal){
              count++;
              for(int j=i+1;j<compressedbit.size();j++){
                if(compressedbit[j] == currentVal){
                     count++;
                }else{
                     break;
                }
              }
              if(count == 1){
                compressedbit[i] = "00000";
              }else if(count == 2){
                compressedbit[i] = "00001";
                compressedbit.erase(compressedbit.begin()+i+1);
              }else if(count == 3){
                compressedbit[i] = "00010";
                compressedbit.erase(compressedbit.begin()+i+1);
                compressedbit.erase(compressedbit.begin()+i+1);
              }else if(count == 4){
                compressedbit[i] = "00011";
                compressedbit.erase(compressedbit.begin()+i+1);
                compressedbit.erase(compressedbit.begin()+i+1);
                compressedbit.erase(compressedbit.begin()+i+1);
              }
              step = count;
         }
    }
}

void oneBitMiss(map<string,string> &dict,vector<string> &compressedbit){
    for(int i=0;i<compressedbit.size();i++){
        string ins = compressedbit[i];
        string dictIndex;
        string position;
        if(ins.length() == 32){
            for (auto const& x : dict)
                {
                    string xorString = XORconvert(ins,x.second);
                    if (countOnes(xorString) == 1)
                    {
                        dictIndex = x.first;
                        for (int j = 0; j < xorString.length(); j++)
                        {
                            if (xorString[j] == '1')
                            {
                                position = decimalToBinary(j);
                                position = string(5-position.size(),'0')+position;
                            }
                        }
                        compressedbit[i] = "010"+position+dictIndex;
                        break;
                    }
                }
        }
    }
}

void twoBitMiss(map<string,string> &dict,vector<string> &compressedbit){
    for(int i=0;i<compressedbit.size();i++){
        string ins = compressedbit[i];
        string dictIndex;
        string position;
        string secPosition;
        if(ins.length() == 32){
            for (auto const& x : dict)
                {
                    string xorString = XORconvert(ins,x.second);
                    if (countOnes(xorString) == 2)
                    {
                        dictIndex = x.first;
                        for (int j = 0; j < xorString.length(); j++)
                        {
                            if (xorString[j] == '1')
                            {
                                position = decimalToBinary(j);
                                position = string(5-position.size(),'0')+position;
                                break;
                            }
                        }
                        for (int k = 0; k < xorString.length(); k++)
                        {
                            if (xorString[k] == '1' && k != binaryToDecimal(stoi(position)))
                            {
                                secPosition = decimalToBinary(k);
                                secPosition = string(5-secPosition.size(),'0')+secPosition;
                                break;
                            }
                        }
                        compressedbit[i] = "100"+position+secPosition+dictIndex;
                        break;
                    }
                }
        }
    }
}

void twoBitMissConsecutive(map<string,string> &dict,vector<string> &compressedbit){
    for(int i=0;i<compressedbit.size();i++){
        string ins = compressedbit[i];
        string dictIndex;
        string position;
        if(ins.length() == 32){
            for (auto const& x : dict)
                {
                    string xorString = XORconvert(ins,x.second);
                    if (countOnes(xorString) == 2)
                    {
                        for (int j = 0; j < xorString.length(); j=j+1)
                        {
                            if(xorString[j] == '1'){
                               if (xorString[j+1] == '1')
                                {
                                    position = decimalToBinary(j);
                                    position = string(5-position.size(),'0')+position;
                                    dictIndex = x.first;
                                    compressedbit[i] = "011"+position+dictIndex;
                                    break;
                                }
                            }
                        }
                    }
                }
        }
    }
}

void genBitMask(map<string,string> &dict,vector<string> &compressedbit){
    for(int i=0;i<compressedbit.size();i++){
        string ins = compressedbit[i];
        string dictIndex;
        string position;
        string bitMask;
        if(ins.length() == 32){
            for (auto const& x : dict)
                {
                    string xorString = XORconvert(ins,x.second);
                    if (countOnes(xorString) == 4 || countOnes(xorString) == 3 || countOnes(xorString) == 2)
                    {
                        bitMask = xorString;
                        while(bitMask.front() == '0'){
                            bitMask = bitMask.substr(1);
                        }
                        while(bitMask.back() == '0'){
                            bitMask.pop_back();
                        }
                        if (bitMask.length() == 4)
                        {
                            dictIndex = x.first;
                            for (int j = 0; j < xorString.length(); j++)
                            {
                                if (xorString[j] == '1')
                                {
                                    position = decimalToBinary(j);
                                    position = string(5-position.size(),'0')+position;
                                    break;
                                }
                            }
                            compressedbit[i] = "001"+position+bitMask+dictIndex;
                            break;
                        }
                        else if(bitMask.length() == 3 && xorString[29]== '1' && xorString[31] == '1')
                        {
                            dictIndex = x.first;
                            bitMask = "0"+bitMask;
                            position = decimalToBinary(28);
                            position = string(5-position.size(),'0')+position;
                            
                            compressedbit[i] = "001"+position+bitMask+dictIndex;
                            break;
                        }
                        else if (bitMask.length() == 3 || bitMask.length() == 2)
                        {
                            dictIndex = x.first;
                            bitMask = bitMask + "0";
                            for (int k = 0; k < xorString.length(); k++)
                            {
                                if (xorString[k] == '1')
                                {
                                    position = decimalToBinary(k);
                                    position = string(5-position.size(),'0')+position;
                                    break;
                                }
                            }
                            compressedbit[i] = "001"+position+bitMask+dictIndex;
                            break;
                        }
                        
                    }
                }
        }
    }
}

void genFinal(vector<string> &compressedbit){
    for(int i=0;i<compressedbit.size();i++){
        string ins = compressedbit[i];
        if(ins.length() == 32){
            compressedbit[i] = "110"+ins;
        }
    }
}

void genCompress(map<string, string> &originalDict, vector<string> &originalBits,vector<string> &compressedbit){
    frequencyCounter(originalBits,originalDict);
    directMap(originalDict,originalBits,compressedbit);
    runLengthEncode(compressedbit);
    oneBitMiss(originalDict,compressedbit);
    twoBitMissConsecutive(originalDict,compressedbit);
    genBitMask(originalDict,compressedbit);
    twoBitMiss(originalDict,compressedbit);
    genFinal(compressedbit);
}

/* Compression End*/

vector<string> makeOutput(vector<string> &compressedbit,map<string, string> &originalDict){
    vector<string> compressedOutput;
    string bitCode;
    for(int i=0;i<compressedbit.size();i++){
        bitCode = bitCode + compressedbit[i];
    }
    for(int j=0;j<bitCode.length();j=j+32){
        compressedOutput.push_back(bitCode.substr(j,32));
    }

    string lastItem = compressedOutput.back();
    compressedOutput.pop_back();
    while(lastItem.length() != 32){
        lastItem = lastItem + "1";
    }
    compressedOutput.push_back(lastItem);

    compressedOutput.push_back("xxxx");
    for (auto const& x : originalDict)
    {
        compressedOutput.push_back(x.second);
    }

    return compressedOutput;
}

void makeFile(vector<string> &vectorOutput, string fileName){
    ofstream file;
    file.open (fileName);
    for(int i=0;i<vectorOutput.size();i++){
        file << vectorOutput[i] << endl;
    }
    file.close();
}

int main(int argc, char* argv[])
{

/* Compression */
if (stoi(argv[1]) ==1){
    string originalFileName;
    vector<string> originalBits;
    map<string, string> originalDict;
    vector<string> compressedbit;
    storeFile("original.txt", originalBits);
    genCompress(originalDict,originalBits,compressedbit);
    vector<string> final =  makeOutput(compressedbit,originalDict);
    makeFile(final,"cout.txt");
}
/* Decompression */
else if (stoi(argv[1]) == 2){
    string compressBits;
    map<string, string> compressDict;
    vector<string> decompressedIns;
    genBitsAndDict("compressed.txt",compressBits,compressDict);
    genOriginal(compressBits,compressDict,decompressedIns);
    makeFile(decompressedIns,"dout.txt");
}
    return 0;
}

