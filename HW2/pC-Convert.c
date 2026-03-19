#include <stdio.h>
#include <string.h>

#define MAXLEN 10005
#define MAXTOK 10005

// 把「讀進來的東西」標準化
typedef struct Token {
    int type;   // 1 = number, 2 = operator, 3 = parentheses
    char ch;    // operator or parentheses
    int num;    // number
} Token;

Token input[MAXTOK];
int input_len = 0;

int prec(char c);
int is_right_assoc(char c);
void tokens_to_prefix(char *out, size_t outsz);
void init();
void getInput();
void printResult(int res);

// 以 Token array 做 reverse-trick：infix -> prefix 
int prec(char c){
    if (c=='+' || c=='-') return 1;
    if (c=='*' || c=='/') return 2;
    return 0;
}

int is_right_assoc(char c){
    (void)c; return 0;
}

// 將全域 input[] / input_len 轉成 prefix 字串，輸出到 out
void tokens_to_prefix(char *out, size_t outsz){
    // 1. 反轉 token 並交換括號
    Token rev[MAXTOK];
    int n = input_len;
    for (int i=0;i<n;i++){
        rev[i] = input[n-1-i];
        if (rev[i].type==3){
            if (rev[i].ch=='(') rev[i].ch = ')';
            else if (rev[i].ch==')') rev[i].ch = '(';
        }
    }

    // 2. (反轉後的) infix -> postfix
    Token post[MAXTOK];
    int post_len = 0;
    char st[MAXTOK]; int top = 0;

    for (int i=0;i<n;i++){
        if (rev[i].type==1){ // number
            post[post_len++] = rev[i];
        } else if (rev[i].type==3 && rev[i].ch=='('){
            st[top++] = '(';
        } else if (rev[i].type==3 && rev[i].ch==')'){
            while (top>0 && st[top-1] != '('){
                post[post_len].type = 2;
                post[post_len].ch   = st[--top];
                post_len++;
            }
            if (top>0 && st[top-1]=='(') --top; 
        } else if (rev[i].type==2){ // operator
            char cur = rev[i].ch;
            // 同優先序不要 pop，只在嚴格大於時 pop
            while (top>0 && st[top-1] != '(' &&
                   prec(st[top-1]) > prec(cur)){
                post[post_len].type = 2;
                post[post_len].ch   = st[--top];
                post_len++;
            }
            st[top++] = cur;
        }
    }
    // 把 stack 中剩餘的運算子全部 pop
    while (top>0){
        post[post_len].type = 2;
        post[post_len].ch   = st[--top];
        post_len++;
    }

    // 3. postfix 反轉成 prefix
    for (int i=0;i<post_len/2;i++){
        Token tmp = post[i];
        post[i] = post[post_len-1-i];
        post[post_len-1-i] = tmp;
    }

    // 4. 序列化成字串（空白分隔）
    size_t pos = 0;
    for (int i=0;i<post_len && pos<outsz; i++){
        if (post[i].type==1){
            pos += snprintf(out+pos, outsz-pos, "%d", post[i].num);
        }
        else{
            pos += snprintf(out+pos, outsz-pos, "%c", post[i].ch);
        } 
        if (i+1<post_len){
            pos += snprintf(out+pos, outsz-pos, " ");
        }     
    }
    if (pos<outsz) out[pos] = '\0';
    else out[outsz-1] = '\0';
}

// infix tokens -> postfix -> 計算結果
int apply_op(int a, int b, char op){
    switch(op){
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b; 
    }
    return 0;
}

// infix to postfix，回傳 out 的長度
int infix_to_postfix(Token *out){
    int m = 0;
    char st[MAXTOK]; int top = 0;     

    for (int i = 0; i < input_len; ++i){
        if (input[i].type == 0){
            continue;                  // 跳過空 token（防止多重空白）
        } else if (input[i].type == 1){ 
            out[m++] = input[i];
        } else if (input[i].type == 3 && input[i].ch == '('){
            st[top++] = '(';
        } else if (input[i].type == 3 && input[i].ch == ')'){
            while (top > 0 && st[top-1] != '('){
                out[m].type = 2; out[m].ch = st[--top]; ++m;
            }
            if (top > 0 && st[top-1] == '(') --top; 
        } else if (input[i].type == 2){             
            char cur = input[i].ch;
            while (top > 0 && st[top-1] != '(' &&
                   prec(st[top-1]) >= prec(cur)){
                out[m].type = 2; out[m].ch = st[--top]; ++m;
            }
            st[top++] = cur;
        }
    }
    while (top > 0){
        out[m].type = 2; out[m].ch = st[--top]; ++m;
    }
    return m;
}

int eval_postfix(const Token *post, int m){
    int vs[MAXTOK], vz = 0;          
    for (int i = 0; i < m; ++i){
        if (post[i].type == 1){
            vs[vz++] = post[i].num;
        } else { 
            int b = vs[--vz], a = vs[--vz];
            vs[vz++] = apply_op(a, b, post[i].ch);
        }
    }
    return vz ? vs[vz-1] : 0;
}

int eval_result(void){
    Token post[MAXTOK];
    int m = infix_to_postfix(post);
    return eval_postfix(post, m);
}

// 清乾淨 token，把 input[] 全部歸零
void init() {
    for (int i = 0; i < MAXTOK; i++) {
        input[i].type = 0;
        input[i].ch = '\0';
        input[i].num = 0;
    }
}

// 輸入
void getInput() {
    char line[MAXLEN];
    fgets(line, sizeof(line), stdin);
    int len = (int)strlen(line);
    for (int i = 0; i < len; i++) {
        char c = line[i];
        if (c == ' ')  // 遇到空白視為 換到下一個 token 格子
            input_len += 1;
        else if ('0' <= c && c <= '9') {
            input[input_len].type = 1; // number
            input[input_len].num = input[input_len].num * 10 + (c - '0');
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            input[input_len].type = 2; // operator
            input[input_len].ch = c;
        } else if (c == '(' || c == ')') {
            input[input_len].type = 3; // parentheses
            input[input_len].ch = c;
        } else {
            break; // end of line
        }
    }
    ++input_len;
}

// 印出答案
void printResult(int res) {
    printf("%d\n", res);
    char buf[MAXLEN * 2];
    tokens_to_prefix(buf, sizeof(buf));
    printf("%s\n", buf);
}

// main function
int main() {
    init();
    getInput();
    int res = eval_result();     // 計算整數結果
    printResult(res);            // 第一行結果、第二行 prefix
    return 0;
}
