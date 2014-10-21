//
//  JsonUtil.h
//  CheatGame
//
//  Created by yankai.peng on 14-10-17.
//
//

#ifndef __CheatGame__JsonUtil__
#define __CheatGame__JsonUtil__

#include <stdio.h>
#include "cocos2d.h"
#include "json.h"

using namespace std;
using namespace cocos2d;
using namespace extension;


/**
 希望改进功能：
    1：增加容错功能、异常抛出功能。
    2：进一步优化、查错解析还原过程。
    3: 兼容coco2dx2.x & 3.x版本
    
 **/

class JsonUtil{
    
public:
    
    /**
     解析json以cocos2dx容器(CCArrar & CCDictionary)储存
     */
    static CCObject* jsonValue(const char *_char);
    
    /**
     将以cocos2dx容器(CCArrar & CCDictionary)储存的json数据还原为为json字符串
     @par stringBuf  传递一个空字符串，得到还原后的json字符串
     */
    static void encodeJson(CCObject *jsonData,string *stringBuf);
private:
    
    static void decodeJson(Json *json,CCObject *farther,int fartherType);
    
};

#endif /* defined(__CheatGame__JsonUtil__) */
