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
#include <string.h>
#include "lib_cjson.h"

const char* config_path = "./config.json";

/**
 * @brief 用于生产默认的json配置文件
 *
 * @return 返回json结构体
 */
cJSON* json_create_default(void)
{
	FILE*  fp = NULL;
    cJSON* cjson_default = NULL;
    cJSON* cjson_lora_para = NULL;
    cJSON* cjson_earth_control_para = NULL;
    cJSON* cjson_earth_line_para = NULL;
    cJSON* cjson_earth_pile_para = NULL;
    cJSON* cjson_mqtt_para = NULL;

    cjson_default = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_default, "config_name", "cjson_default");

    cjson_lora_para = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_lora_para, "mqtt_ip", "192.168.0.2");
    cJSON_AddStringToObject(cjson_lora_para, "mqtt_server_port", "4001");
    cJSON_AddItemToObject(cjson_default, "lora_para", cjson_lora_para);

    /* 添加一个数组类型的JSON数据(添加一个链表节点) */
    cjson_earth_control_para = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_earth_control_para, "dev_name", "earth_control");
    cJSON_AddNumberToObject(cjson_earth_control_para, "number", 1);
    cJSON_AddNumberToObject(cjson_earth_control_para, "cycle_time", 3600);
    cJSON_AddStringToObject(cjson_earth_control_para, "mqtt_ip", "localhost");
    cJSON_AddStringToObject(cjson_earth_control_para, "mqtt_server_port", "1883");
    cJSON_AddItemToObject(cjson_default, "earth_control", cjson_earth_control_para);

	cjson_earth_line_para = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_earth_line_para, "dev_name", "earth_line");
    cJSON_AddNumberToObject(cjson_earth_line_para, "number", 1);
    cJSON_AddStringToObject(cjson_earth_line_para, "lora_id", "ABCDEF1234567890");
    cJSON_AddStringToObject(cjson_earth_line_para, "rfid", "ABCDEF1234");
    cJSON_AddItemToObject(cjson_default, "earth_line", cjson_earth_line_para);

    cjson_earth_pile_para = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_earth_pile_para, "dev_name", "earth_pile");
    cJSON_AddNumberToObject(cjson_earth_pile_para, "number", 1);
    cJSON_AddStringToObject(cjson_earth_pile_para, "rfid", "ABCDEF1234");
    cJSON_AddItemToObject(cjson_default, "earth_pile", cjson_earth_pile_para);

    cjson_mqtt_para = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_mqtt_para, "dev_name", "mqtt");
    cJSON_AddNumberToObject(cjson_mqtt_para, "number", 1);
    cJSON_AddStringToObject(cjson_mqtt_para, "rfid", "ABCDEF1234");
    cJSON_AddItemToObject(cjson_default, "mqtt", cjson_mqtt_para);

    /* 写入default配置    */
	fp = fopen(config_path, "w");
	if(!fp) {
        printf("fopen error!\n");
        return NULL;
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
int json_load_str(const char *fpath, char *str)
{
	long length;
	FILE *fp = fopen(fpath, "rb");
	if (!fp){
		printf("%s is not exit.\n",fpath);
		return 0;
	}

	//指向fp尾，并汇报位置，此处小于0，则fp无效，pos为fp总长
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	if (ftell(fp) < 0) {
		printf("%s is not efficient.\n",fpath);
		fclose(fp);
		return 0;
	}

	//重新指向fp头
	fseek(fp, 0, SEEK_SET);

	//读fp至str中
	if (fread(str, 1, length, fp) < 1) {
		if (ferror(fp)) {
			printf("%s read failed.\n",fpath);
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);

	//压缩str，用于去除注释
	cJSON_Minify(str);
	return 1;
}

static void json_str_prase(cJSON* cjson_data)
{
	const char* str = NULL;
	str = cJSON_Print(cjson_data);
	printf("%s\n", str);
	printf("-----------------------------------------------\n");
	cJSON* config_name = cJSON_GetObjectItem(cjson_data, "config_name");
	printf("config_name: %s\n", config_name->valuestring);

	cJSON* lora_para = cJSON_GetObjectItem(cjson_data, "lora_para");
	cJSON* lora_ip = cJSON_GetObjectItem(lora_para, "mqtt_ip");
	cJSON* lora_port = cJSON_GetObjectItem(lora_para, "mqtt_server_port");
	printf("lora_para : lora_ip :%s\n", lora_ip->valuestring);
	printf("lora_para : lora_port :%s\n", lora_port->valuestring);
}


/**
 * @brief   用于解析读入的json语句字符串str
 */
cJSON* json_work(void)
{
	char str[1024];
	int result = 0;
	cJSON* cjson_data = NULL;

	memset(str, '\0', sizeof(str));
	result = json_load_str(config_path, str);
	if (result) {
		cjson_data = cJSON_Parse(str);
	}else {
		printf("%s is not exit,and create config.json as default.\n",config_path);
		cjson_data = json_create_default();
	}
	if (cjson_data == NULL)
		return NULL;

	json_str_prase(cjson_data);

	// 释放配置
	return cjson_data;
}

/**
 * @brief 对一层嵌套或者二层嵌套进行更改
 *
 * @param index		     嵌套层
 * @param cjson_data	 读入的配置文件
 * @param string1		     要更改的元素名称嵌套1
 * @param string2		     要更改的元素名称嵌套2
 * @param newitem	     新录入的json结构体
 */
void json_edit(int index, cJSON* cjson_data, char** string, cJSON* newitem)
{
	cJSON* prechange = NULL;
	prechange = cJSON_GetObjectItem(cjson_data, *string);
	for (int i = 1; i < index; i++) {
		prechange = cJSON_GetObjectItem(prechange, *(string+i));
	}
	cJSON_ReplaceItemInObject(prechange, *(string+index-1), newitem);

//	cJSON* data = cJSON_GetObjectItem(cjson_data, *string);
//	data = cJSON_GetObjectItem(data, *(string+1));
	printf("ip: %s\n", prechange->valuestring);
	cJSON_Delete(prechange);
}

void edit_str(int index, char** string)
{
	printf("%s\n", *string);
	for (int i = 1; i < index; i++) {
		printf("%s\n", *(string+i));
	}
	printf("%s\n", *(string+index-1));
}

int main(void)
{
	cJSON* cjson_data = json_work();
	printf("%s\n", cJSON_Print(cjson_data));
	printf("-----------------------------\n");
	char* str[] = {"lora_para", "mqtt_ip"};
	char** ch = str ;
	json_edit(2, cjson_data, ch, cJSON_CreateString("1.0.0.1"));
	cJSON_Delete(cjson_data);
//	edit_str(2, ch);
	return 0;
}
