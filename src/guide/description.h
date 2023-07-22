/**
 * @file description.h
 * @author Egor Kondratov(https://github.com/mavissig)
 * @brief Основной файл документации проекта для формирования doxygen отчета
 * @version 0.1
 * @date 2023-05-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/**
* @mainpage Программа для просмотра 3D моделей в каркасном виде (3D Viewer)
* @details Программа разработана на языке Си стандарта C11 с использованием компилятора gcc. 
* В проекте использованы дополнительные библиотеки и модули QT
*
*
* Программа разработана в соответствии с принципами структурного программирования
* При написании кода придерживались Google Style
* 
* Основные части проекта:
* - @ref parser.h "Парсинг файла"
* - @ref calculate.h "Расчет смещения"
* - @ref viewer.h "Отрисовка"
*
* Основные цели для работы с проетом:
* - установка:
*   - Для установки проекта и сборки тестов используйте цель all
*   @code {.c}
*   make all
*   @endcode
*
*   - Для установки проекта без сборки тестов используте цель install
*   @code {.c}
*   make install
*   @endcode
* - Удаление:
*   - Для удаления программы использйте цель uninstall
*   @code {.c}
*   make uninstall
*   @endcode
* 
* 
* - Запуск программы после установки:
*   - Для запуска программы используйте цель run
*   @code {.c}
*   make run
*   @endcode
*  
* - Тесты:
*   - Для сборки тестов без установки используйте цель tests
*   @code {.c}
*   make tests
*   @endcode
*   - Для запуска тестов используйте цель run_test
*   @code {.c}
*   make run_test
*   @endcode
* 
* 
* 
* 
*
* @author Egor Kondratov(https://github.com/mavissig)
*/

/**
 * @brief Основные цели Makefile
 * 
 * @page Makefile
 * Основные цели Makefile
 * - all - Запускает тесты и устанавливает программу
 * - install - устанавливает программу в директорию C8_3DViewer_v1.0-1/src/build
 * - uninstall - удаляет проект с мвшины
 * - clean - производит очистку проекта, подробнее описано @ref clean "здесь"
 * - dvi - формирует и открывает документацию
 * - dist - создаёт архив проекта
 * - tests - собирает тесты
 * - gcov_report формирует репорт с покрытием тестов
 * 
 */

/**
 * @brief Список файлов, которые будут удалены при использовании make clean
 * 
 * @page clean
 * - $(CTEST_DIR)*.dSYM/ 
 * - .DS_Store *.o
 * - $(CREPORT_DIR)*.gcda 
 * - $(CREPORT_DIR)*.gcno 
 * - *.a $(CRUNNER)
 * - $(CREPORT_DIR)report 
 * - $(CREPORT_DIR)*.info 
 * - $(CREPORT_DIR)*.index 
 * - $(CREPORT_DIR)gcov_report
 * - $(CBUILD_DIR)*.dSYM 
 * - *.out vgcore* 
 * - *.data 
 * - *.heap 
 * - $(CLIB_DIR)*.a 
 * - $(CBUILD_DIR)*.dSYM
 * - *.zip* 
 * - *.tar* 
 * - $(CBUILD_DIR)ui_*.h 
 * - $(CBUILD_DIR)moc_*.h 
 * - $(CREPORT_DIR)*.dSYM 
 * - $(CBUILD_DIR)*.tmp
 * - $(CBUILD_DIR)Makefile 
 * - $(CBUILD_DIR)*.cpp 
 * - $(CBUILD_DIR).qmake.stash 
 * - $(CBUILD_DIR)*.o 
 * - *.gcno 
 * - *.gcda
 * - build-Viewer_v1_0-* 
 * - $(CLOGS_DIR)*.old 
 * - $(CBUILD_DIR)*.so
 * 
 */