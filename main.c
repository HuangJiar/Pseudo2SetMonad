#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lang.h"
#include "lexer.h"
#include "parser.h"

extern struct cmd * root;
int yyparse();

#define MAX_LINE_LEN 1024
#define MAX_LINES 10000
#define INDENT_UNIT 4

// 检查输出缓冲区最后是否已经有左花括号
int last_char_is_brace(const char* output) {
    int len = strlen(output);
    // 从后往前找第一个非空白字符
    for (int i = len - 1; i >= 0; i--) {
        if (output[i] == ' ' || output[i] == '\t' || output[i] == '\n' || output[i] == '\r') {
            continue;
        }
        return (output[i] == '{');
    }
    return 0;
}

char* process_indentation(FILE* input) {
    char line[MAX_LINE_LEN];
    char* output = (char*)malloc(MAX_LINES * MAX_LINE_LEN);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    output[0] = '\0';
    
    int indent_stack[1000];
    int brace_inserted[1000];  // 记录每一层是否插入了花括号
    int stack_top = 0;
    indent_stack[0] = 0;
    brace_inserted[0] = 0;
    
    while (fgets(line, MAX_LINE_LEN, input) != NULL) {
        // 跳过空行和只有空白的行
        int is_blank = 1;
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r') {
                is_blank = 0;
                break;
            }
        }
        if (is_blank) continue;
        
        // 计算当前行的缩进（以空格数计）
        int curr_indent = 0;
        int i = 0;
        while (line[i] == ' ' || line[i] == '\t') {
            if (line[i] == ' ') {
                curr_indent++;
            } else if (line[i] == '\t') {
                curr_indent += INDENT_UNIT;
            }
            i++;
        }
        
        // 缩进必须是 INDENT_UNIT 的倍数
        if (curr_indent % INDENT_UNIT != 0) {
            fprintf(stderr, "Error: Indentation must be multiple of %d spaces\n", INDENT_UNIT);
            free(output);
            exit(1);
        }
        
        int curr_level = curr_indent / INDENT_UNIT;
        int prev_level = indent_stack[stack_top];
        
        // 如果缩进增加
        if (curr_level > prev_level) {
            if (curr_level != prev_level + 1) {
                fprintf(stderr, "Error: Indentation increased by more than one level\n");
                free(output);
                exit(1);
            }
            // 检查是否需要插入花括号
            int need_brace = !last_char_is_brace(output);
            if (need_brace) {
                strcat(output, "{\n");
            }
            indent_stack[++stack_top] = curr_level;
            brace_inserted[stack_top] = need_brace;
        }
        // 如果缩进减少，只关闭我们插入的花括号
        else if (curr_level < prev_level) {
            while (stack_top > 0 && indent_stack[stack_top] > curr_level) {
                if (brace_inserted[stack_top]) {
                    strcat(output, "}\n");
                }
                stack_top--;
            }
            if (indent_stack[stack_top] != curr_level) {
                fprintf(stderr, "Error: Invalid indentation level\n");
                free(output);
                exit(1);
            }
        }
        
        // 添加当前行（去掉缩进）
        // 去掉行尾的冒号（Python风格）
        char* line_content = &line[i];
        int content_len = strlen(line_content);
        // 去掉尾部的换行和空白
        while (content_len > 0 && (line_content[content_len-1] == '\n' || 
                                    line_content[content_len-1] == '\r' || 
                                    line_content[content_len-1] == ' ' || 
                                    line_content[content_len-1] == '\t')) {
            content_len--;
        }
        // 添加处理后的行
        strncat(output, line_content, content_len);
        strcat(output, "\n");
    }
    
    // 在最后只关闭我们插入的花括号
    while (stack_top > 0) {
        if (brace_inserted[stack_top]) {
            strcat(output, "}\n");
        }
        stack_top--;
    }
    
    return output;
}

int main(int argc, char **argv) {
    // 处理缩进，将缩进转换为花括号
    char* processed_input = process_indentation(stdin);
    fclose(stdin);
    
    // 调试：输出处理后的代码
    fprintf(stderr, "===== Processed Input =====\n%s\n===== End =====\n", processed_input);
    
    // 将处理后的字符串写入临时文件
    FILE* temp = tmpfile();
    if (temp == NULL) {
        fprintf(stderr, "Failed to create temporary file\n");
        free(processed_input);
        return 1;
    }
    
    fprintf(temp, "%s", processed_input);
    free(processed_input);
    rewind(temp);
    
    // 让 lexer 从临时文件读取
    yyin = temp;
    int parse_result = yyparse();
    fclose(temp);
    
    // 检查解析结果
    if (parse_result != 0) {
        fprintf(stderr, "Parsing failed with error code %d\n", parse_result);
        return 1;
    }
    
    if (root == NULL) {
        fprintf(stderr, "Error: root is NULL after parsing\n");
        return 1;
    }
    
    print_cmd(root);
    printf("\n");
    
    return 0;
}
