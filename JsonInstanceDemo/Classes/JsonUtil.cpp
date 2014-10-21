//
//  JsonUtil.cpp
//  CheatGame
//
//  Created by yankai.peng on 14-10-17.
//
//

#include "JsonUtil.h"


using namespace std;
cocos2d::CCObject * JsonUtil::jsonValue(const char *_char){
    
    Json *jsonObj=Json_create(_char);
    
    if(jsonObj==NULL) return NULL;
    
    CCObject *obj;
    switch (jsonObj->type) {

        case Json_Array:
            obj=CCArray::create();
            decodeJson(jsonObj->child, obj, Json_Array);
            break;
            
        case Json_Object:
            obj=CCDictionary::create();
            decodeJson(jsonObj->child,obj , Json_Object);
            break;
        default:
            return  NULL;
    }
    return obj;
}


void JsonUtil::decodeJson(Json *json,CCObject *farther,int fartherType){
    
    Json *tmpJson=json;
    while (true) {
        
        
        switch (tmpJson->type) {
                
            case Json_Array:
                if(tmpJson->child==NULL)break;
                
                if(fartherType==Json_Object){
                    
                    CCArray *arr=CCArray::create();
                    ((CCDictionary *)farther)->setObject(arr, tmpJson->name);
                    decodeJson(tmpJson->child, arr, Json_Array);
                    
                }else if (fartherType==Json_Array){
                    
                    CCArray *arr=CCArray::create();
                    ((CCArray *)farther)->addObject(arr);
                    decodeJson(tmpJson->child, arr, Json_Array);
                }
                
                break;
            case Json_Object:
                if(tmpJson->child==NULL)break;
                if(fartherType==Json_Object){
                    
                    CCDictionary *dic=CCDictionary::create();
                    ((CCDictionary *)farther)->setObject(dic, tmpJson->name);
                    decodeJson(tmpJson->child, dic, Json_Object);
                    
                }else if (fartherType==Json_Array){
                    
                    CCDictionary *dic=CCDictionary::create();
                    ((CCArray *)farther)->addObject(dic);
                    decodeJson(tmpJson->child, dic, Json_Object);
                }
                break;
            case Json_False:
                
                if(fartherType==Json_Array){
                    
                    ((CCArray *) farther)->addObject(CCBool::create(false));
                    
                }else if(fartherType==Json_Object){
                    ((CCDictionary *)farther)->setObject(CCBool::create(false), tmpJson->name);
                    
                }
                
                break;
                
            case Json_True:
                
                if(fartherType==Json_Array){
                    
                    ((CCArray *) farther)->addObject(CCBool::create(true));
                    
                }else if(fartherType==Json_Object){
                    ((CCDictionary *)farther)->setObject(CCBool::create(true), tmpJson->name);
                    
                }
                break;
            case Json_NULL:
                if(fartherType==Json_Array){
                    
                    ((CCArray *) farther)->addObject(CCString::createWithFormat("null"));
                    
                }else if(fartherType==Json_Object){
                    ((CCDictionary *)farther)->setObject(CCString::createWithFormat("null"), tmpJson->name);
                    
                }
                break;
            
            case Json_Number:
                
                if(fartherType==Json_Array){
                    
                    ((CCArray *) farther)->addObject(CCFloat::create(tmpJson->valuefloat));
                    
                }else if(fartherType==Json_Object){
                    ((CCDictionary *)farther)->setObject(CCFloat::create(tmpJson->valuefloat), tmpJson->name);
                    
                }
                break;
            case Json_String:
                
                if(fartherType==Json_Array){
                    
                    ((CCArray *) farther)->addObject(CCString::createWithFormat("%s",tmpJson->valuestring));
                    
                }else if(fartherType==Json_Object){
                    ((CCDictionary *)farther)->setObject(CCString::createWithFormat("%s",tmpJson->valuestring), tmpJson->name);
                    
                }
                
                break;
        }
        
        
        tmpJson=tmpJson->next;
        if(tmpJson==NULL) break;
        
    }
    
}

void JsonUtil::encodeJson(CCObject *jsonData,string *stringBuf){
    
    if(typeid(*jsonData)==typeid(CCArray)){
        
        stringBuf->append("[");
        
        CCArray *arr=(CCArray *)jsonData;
        for (int i=0; i<arr->count(); i++) {
            
            CCObject *t=arr->objectAtIndex(i);
            encodeJson(t, stringBuf);
            if(i<arr->count()-1) stringBuf->append(",");
        }
        
        stringBuf->append("]");
        
    }else if(typeid(*jsonData)==typeid(CCDictionary)){
        
        stringBuf->append("{");
        
        CCDictionary *dic=(CCDictionary *)jsonData;
        for (int i=0; i<dic->count(); i++) {
            
            string k=((CCString *)dic->allKeys()->objectAtIndex(i))->getCString();
            CCObject *v= dic->objectForKey(k.c_str());
            
            if (typeid(*v)==typeid(CCString)){
                
                CCString *str=(CCString *)v;
                if(str->isEqual(CCString::create("null"))){
                    
                    stringBuf->append("\""+k+"\""+":"+"null");
                    
                }else{
                    stringBuf->append("\""+k+"\""+":"+"\""+str->getCString()+"\"");
                }
                
            }else if (typeid(*v)==typeid(CCFloat)){
                
                float f=((CCFloat *)v)->getValue();
                stringBuf->append("\""+k+"\""+":"+CCString::createWithFormat("%f",f)->getCString());;
                
            }else if (typeid(*v)==typeid(CCBool)){
                
                bool b=((CCBool *)v)->getValue();
                if(b){
                    stringBuf->append("\""+k+"\""+":"+"true");
                }else{
                    stringBuf->append("\""+k+"\""+":"+"false");
                }
            }else if (typeid(*v)==typeid(CCArray)){
                
                stringBuf->append("\""+k+"\""+":");
                encodeJson(v, stringBuf);
                
            }else if (typeid(*v)==typeid(CCDictionary)){
                stringBuf->append("\""+k+"\""+":");
                encodeJson(v, stringBuf);
            }
            
            if(i<dic->count()-1) stringBuf->append(",");
            
        }
        
        stringBuf->append("}");
    }else if (typeid(*jsonData)==typeid(CCString)){
        
        CCString *str=(CCString *)jsonData;
        if(str->isEqual(CCString::create("null"))){
            
            stringBuf->append("null");
            
        }else{
            string t=str->getCString();
            stringBuf->append("\""+t+"\"");
        }
        
    }else if (typeid(*jsonData)==typeid(CCFloat)){
        
        float f=((CCFloat *)jsonData)->getValue();
        stringBuf->append(CCString::createWithFormat("%f",f)->getCString());;
        
    }else if (typeid(*jsonData)==typeid(CCBool)){
        
        bool b=((CCBool *)jsonData)->getValue();
        if(b){
            stringBuf->append("true");
        }else{
            stringBuf->append("false");
        }
    }
}

