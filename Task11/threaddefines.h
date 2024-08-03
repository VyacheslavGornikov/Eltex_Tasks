#ifndef _THREADDEFINES_H_
#define _THREADDEFINES_H_

#define STORES_NUM 5 /* Количество магазинов */
#define CUSTOMERS_NUM 3 /* Количество покупателей */
#define MAX_STORE_CAPACITY 10000 /* Максимальное начальное число товаров в магазине */
#define MAX_CUSTOMER_NEED 100000 /* Максимальная потребность покупателя */
#define LOADER_CAPACITY 5000 /* Количество загружаемых в магазин товаров */

/*
 * Структура, описывающая магазин
 */
typedef struct store
{
    int capacity; // Поле, равное количеству товаров в магазине
    int is_busy; // Поле равно 0 - магазин свободен, 1 - занят
}
Store;

#endif

