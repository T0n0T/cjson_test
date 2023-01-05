/*
 ============================================================================
 Name        : 222.c
 Author      : 22
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "lib_cjson.h"

const char *fpath = "./config.txt";

/**
 * @brief 用于生产默认的json配置文件
 *
 * @return 返回json结构体
 */
cJSON* json_create_default(void)
{
	FILE*  fp = NULL;
    cJSON* cjson_default = NULL;
    cJSON* cjson_address = NULL;
    cJSON* cjson_skill = NULL;

    /* 创建一个JSON数据对象(链表头结点) */
    cjson_default = cJSON_CreateObject();

    /* 添加一条字符串类型的JSON数据(添加一个链表节点) */
    cJSON_AddStringToObject(cjson_default, "name", "cjson_default");

    /* 添加一条整数类型的JSON数据(添加一个链表节点) */
    cJSON_AddNumberToObject(cjson_default, "age", 22);

    /* 添加一条浮点类型的JSON数据(添加一个链表节点) */
    cJSON_AddNumberToObject(cjson_default, "weight", 55.5);

    /* 添加一个嵌套的JSON数据（添加一个链表节点） */
    cjson_address = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_address, "country", "China");
    cJSON_AddNumberToObject(cjson_address, "zip-code", 111111);
    cJSON_AddItemToObject(cjson_default, "address", cjson_address);

    /* 添加一个数组类型的JSON数据(添加一个链表节点) */
    cjson_skill = cJSON_CreateArray();
    cJSON_AddItemToArray(cjson_skill, cJSON_CreateString( "C" ));
    cJSON_AddItemToArray(cjson_skill, cJSON_CreateString( "Java" ));
    cJSON_AddItemToArray(cjson_skill, cJSON_CreateString( "Python" ));
    cJSON_AddItemToObject(cjson_default, "skill", cjson_skill);

    /* 添加一个值为 False 的布尔类型的JSON数据(添加一个链表节点) */
    cJSON_AddFalseToObject(cjson_default, "student");

    /* 写入default配置    */
	fp = fopen(fpath, "a");
	if(!fp) {
        printf("fopen error!\n");
        return -1;
    }
	fprintf(fp,"%s", cJSON_Print(cjson_default));
	fclose(fp);

    return cjson_default;
}


/**
 * @brief   用于读入json文件
 * @param fpath	文件路径
 * @param str		存储读取到的字符串
 * @return 0为读取失败，1为读取成功
 */
int load_json_into_str(const char *fpath, char *str)
{
	long length;

	FILE *fp = fopen(fpath, "r");
	if (!fp){
		printf("%s is not exit.\n",fpath);
		return 0;
	}

	//指向fp尾，并汇报位置，此处小于0，则fp无效，pos为fp总长
	fseek(fp, 0L, SEEK_END);
	length = ftell(fp);
	if (ftell(fp) < 0) {
		printf("%s is not efficient.\n",fpath);
		fclose(fp);
		return 0;
	}

	//重新指向fp头
	rewind(fp);

	//读fp至str中
	if (fread(str, length, 1, fp) < 1) {
		if (ferror(fp)) {
			printf("%s read failed.\n",fpath);
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);

	//cjson库压缩str，用于去除注释
	cJSON_Minify(str);
	return 1;
}

/**
 * @brief   用于解析读入的json语句字符串str
 */
void json_parse(void)
{
	char buf[1024];
	const char* str;
	cJSON *cjson_data = NULL;

	if (load_json_into_str(fpath, str)!= 1) {
		printf("%s is not exit,and create config.json as default.\n",fpath);
		cjson_data = json_create_default();
		/* 打印JSON对象(整条链表)的所有数据 */
		str = cJSON_Print(cjson_data);
		printf("%s\n", str);
	}else {
		cjson_data = cJSON_Parse(str);
	}


	if (cjson_data == NULL)
		return;

	// 释放配置
	cJSON_Delete(cjson_data);
}

int main(void)
{
	const char *fpath;
	char* str = NULL;
	json_create_default();
	return 0;
}
