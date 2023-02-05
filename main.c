/**
 * @file      : main.c
 * @brief     : 程序入口文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-02-04 15:00:18
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date       author          description
 *              2023-02-04 huenrong        创建文件
 *
 */

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "./linux_queue/queue.h"

// 测试队列
static queue_t g_test_queue = {0};

static void *consumer_thread(void *arg);

/**
 * @brief  : 程序入口
 * @param  : argc: 输入参数, 参数个数
 * @param  : argv: 输入参数, 参数列表
 * @return : 成功: 0
 *           失败: 其它
 */
int main(int argc, char *argv[])
{
    // 消费者线程ID
    pthread_t consumer_thread_id;

    // 初始化队列
    if (!queue_init(&g_test_queue, 10))
    {
        printf("init test queue fail\n");

        return -1;
    }

    // 创建消费者线程
    if (0 == pthread_create(&consumer_thread_id, NULL, consumer_thread, NULL))
    {
        // 分离线程
        pthread_detach(consumer_thread_id);

        usleep(500 * 1000);
    }
    else
    {
        printf("create consumer thread fail\n");

        return -1;
    }

    // 生产者线程
    while (true)
    {
        uint8_t put_data[10] = {0x11, 0x22, 0x33};
        uint8_t put_data_len = 3;
        printf("put data...\n");
        if (put_data_len != queue_put_data(&g_test_queue, put_data, put_data_len))
        {
            printf("put data to queue fail\n");
        }

        usleep(500 * 1000);
    }

    return 0;
}

/**
 * @brief  消费者线程
 * @param  arg: 输入参数, 线程参数
 * @return NULL
 */
static void *consumer_thread(void *arg)
{
    int ret = -1;
    uint8_t read_data[10] = {0};

    (void)arg;

    printf("this is %s\n", __func__);

    while (true)
    {
        printf("get queue data...\n");
        memset(read_data, 0, sizeof(read_data));
        ret = queue_get_data(&g_test_queue, read_data, 10);
        if (ret > 0)
        {
            printf("get data: ");
            for (uint8_t i = 0; i < ret; i++)
            {
                printf("0x%02X ", read_data[i]);
            }
            printf("\n\n");
        }
        else
        {
            printf("get queue data fail\n");
        }
    }

    return NULL;
}
