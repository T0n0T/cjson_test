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

static int write_file(const char *filename,char *out)
{
    FILE *fp = NULL;

    fp = fopen(filename,"w");
    if(fp == NULL)
    {
        fprintf(stderr,"open file failed\n");
        exit(-1);
    }
    fprintf(fp,"%s",out);

    if(fp != NULL)
        fclose(fp);

    return 0;
}

/**
 * @brief 用于生产默认的json配置文件
 *
 * @return 返回json结构体
 */
cJSON* json_create_default(void)
{
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
//	fp = fopen(config_path, "w");
//	if(!fp) {
//        printf("fopen error!\n");
//        return NULL;
//    }
//	fprintf(fp,"%s", cJSON_Print(cjson_default));
//	fclose(fp);
    write_file(config_path, cJSON_Print(cjson_default));

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
	printf("length: %ld\n",length);
	if (ftell(fp) < 1) {
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
cJSON* json_load(void)
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

	// 释放配置
	printf("----------get config is OK------------\n");
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
void json_edit(int nest, cJSON* cjson_data, char** string, cJSON* newitem)
{
	cJSON* prechange = NULL;

	prechange = cjson_data;
	for (int i = 0; i < nest-1; i++) {
		prechange = cJSON_GetObjectItem(prechange, *(string+i));
	}

	cJSON_ReplaceItemInObject(prechange, *(string+nest-1), newitem);
	write_file(config_path, cJSON_Print(cjson_data));

}

void print_help(void)
{
	printf("cjson script help:\n");
	printf("using \"cjson_test --default\" to parse a config.json file,if config.json is not exit. "
			"Script will create a default config.json.\n");
	printf("using \"cjson_test --edit nest string1 string2...\" to edit the config.json, and"
			"parse the current config.json.\n");
}



int main(int argc, char** argv)
{
	cJSON* cjson_data = NULL;

	if (argc == 1) {
		print_help();
	} else {
		if (!strcmp("--default", argv[1])) {
			cjson_data = json_create_default();
		}else if (!strcmp("--edit", argv[1])) {
			cjson_data = json_load();
			json_edit(atoi(argv[2]), cjson_data, argv+3, cJSON_CreateString(argv[argc-1]));
//		    printf("-----------------------------\n");
//		    printf("argc = %d\n", argc);
//		    printf("argv[2] = %d\n", atoi(argv[2]));
//		    char **ch;
//		    ch = argv + 3;
//		    printf("argv[3] = %s\n", *(ch));
//		    printf("argv[4] = %s\n", *(ch + 1));
//		    printf("argv[5] = %s\n", *(ch + 2));
		}else if (!strcmp("--parse", argv[1])) {
			cjson_data = json_load();
			json_str_prase(cjson_data);
		} else {
			print_help();
		}

		printf("-----------------------------\n");
		printf("%s\n", cJSON_Print(cjson_data));
	}

	return 0;
}
