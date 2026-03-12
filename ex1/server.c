#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>



typedef struct Query {
    char ip[16];
    int port;
    struct Query *next;
} Query;

typedef struct Rule{
    unsigned int ip_start;
    unsigned int ip_end;
    int port_start;
    int port_end;

    char rule_string[64];
    Query *queries;
    struct Rule *next;
} Rule;

typedef struct Request{
    char text[128];
    struct Request *next;
} Request;

Rule *rules = NULL;
Request *requests = NULL;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void addRequest(char *request){
    Request *newReq = malloc(sizeof(Request));

    if(newReq == NULL){
        return;
    }
    strcpy(newReq->text, request);
    newReq->next = NULL;

    if (requests == NULL){
        requests = newReq;
        return;
    }
    Request *current = requests;

    while(current->next != NULL){
        current = current->next;
    }
    current->next = newReq;
}

char *handleListRequests(){
    char *response = malloc(4096);
    response[0] = '\0';

    Request *current = requests;

    while(current != NULL){
        strcat(response, current->text);
        strcat(response, "\n");
        current = current->next;
    }
    return response;
}

char *handleAdd(char *rule){
    char ipPart[128];
    char portPart[128];
    char startIP[64];
    char endIP[64];
    char startPort[16];
    char endPort[16];

    if(sscanf(rule, "%s %s", ipPart, portPart) != 2) {
        return strdup("Invalid rule");
    }

    if(strchr(ipPart, '-') != NULL){
        sscanf(ipPart, "%[^-]-%s", startIP, endIP);

    }
    else{
        strcpy(startIP, ipPart);
        strcpy(endIP, ipPart);


    }
    if(strchr(portPart, '-') != NULL){
        sscanf(portPart, "%[^-]-%s", startPort, endPort);

    }
    else{
        strcpy(startPort, portPart);
        strcpy(endPort, portPart);


    }

    int startPortNum = atoi(startPort);
    int endPortNum = atoi(endPort);

    if (startPortNum < 0 || startPortNum > 65535 || endPortNum < 0 || endPortNum > 65535){
        return strdup("Invalid rule");
    }
    if (startPortNum > endPortNum){
        return strdup("Invalid rule");
    }

    int a;
    int b;
    int c;
    int d;
    unsigned int startIPNum;

    if(sscanf(startIP, "%d.%d.%d.%d", &a, &b, &c, &d) != 4){
        return strdup("Invalid rule");
    }

    if (a<0 || a>255 || b<0 || b>255 || c<0 || c>255 || d<0 || d>255){
        return strdup("Invalid rule");
    }

    startIPNum = a*256*256*256 + b*256*256 + c*256 + d;

    int e;
    int f;
    int g;
    int h;
    unsigned int endIPNum;

    if(sscanf(endIP, "%d.%d.%d.%d", &e, &f, &g, &h) != 4){
        return strdup("Invalid rule");
    }

    if (e<0 || e>255 || f<0 || f>255 || g<0 || g>255 || h<0 || h>255){
        return strdup("Invalid rule");
    }

    endIPNum = e*256*256*256 + f*256*256 + g*256 + h;

    if(startIPNum > endIPNum){
        return strdup("Invalid rule");
    }

    Rule *newRule = malloc(sizeof(Rule));
    if(newRule == NULL){
        return strdup("Invalid rule");
    }

    newRule->ip_start = startIPNum;
    newRule->ip_end = endIPNum;
    newRule->port_start = startPortNum;
    newRule->port_end = endPortNum;
    strcpy(newRule->rule_string, rule);
    newRule->queries = NULL;
    newRule->next = NULL;

    if (rules == NULL){
        rules = newRule;

    }
    else{
        Rule *current = rules;

    while(current->next != NULL){
        current = current->next;
    }
    current->next = newRule;
}




    return strdup("Rule added");
}

char *handleCheck(char *args){
    char ipPart[128];
    char portPart[128];
    if(sscanf(args, "%s %s", ipPart, portPart) != 2) {
        return strdup("Illegal IP address or port specified");
    }

    int port = atoi(portPart);
    if (port < 0 || port > 65535 ){
        return strdup("Illegal IP address or port specified");
    }

    int a;
    int b;
    int c;
    int d;
    unsigned int IPNum;

    if(sscanf(ipPart, "%d.%d.%d.%d", &a, &b, &c, &d) != 4){
        return strdup("Illegal IP address or port specified");
    }

    if (a<0 || a>255 || b<0 || b>255 || c<0 || c>255 || d<0 || d>255){
        return strdup("Illegal IP address or port specified");
    }

    IPNum = a*256*256*256 + b*256*256 + c*256 + d;

    Rule *current = rules;

    while(current != NULL){
        if(IPNum >= current->ip_start && IPNum <= current->ip_end && port >= current->port_start && port <= current->port_end){
            
            Query *newquery = malloc(sizeof(Query));

            if(newquery == NULL){
                return strdup("Connection rejected");
            }
            strcpy(newquery->ip, ipPart);
            newquery->port = port;
            newquery->next = NULL; 

            if(current->queries == NULL){
                current->queries = newquery;
            }
            else{
                Query *q = current->queries;
                while(q->next != NULL){
                    q = q->next;
                }
                q->next = newquery;
            }
            return strdup("Connection accepted");

        }
        current = current->next;

    }




    return strdup("Connection rejected");
}

char *handleDelete(char *rule){
    char ipPart[128];
    char portPart[128];
    if(sscanf(rule, "%s %s", ipPart, portPart) != 2) {
        return strdup("Invalid rule");
    }
    int port = atoi(portPart);
    if (port < 0 || port > 65535 ){
        return strdup("Invalid rule");
    }

    int a;
    int b;
    int c;
    int d;
   // unsigned int IPNum;

    if(sscanf(ipPart, "%d.%d.%d.%d", &a, &b, &c, &d) != 4){
        return strdup("Invalid rule");
    }

    if (a<0 || a>255 || b<0 || b>255 || c<0 || c>255 || d<0 || d>255){
        return strdup("Invalid rule");
    }

   // IPNum = a*256*256*256 + b*256*256 + c*256 + d;

    Rule *current = rules;
    Rule *prev = NULL;

    while(current != NULL){
        if(strcmp(current->rule_string, rule) == 0){
            if(prev==NULL){
                rules = current->next;
            }
            else{
                prev->next = current->next;

            }
            Query *q = current->queries;
            while (q!= NULL){
                Query *temp = q;
                q = q->next;
                free(temp);

            }
            free(current);
            return strdup("Rule deleted");

        }
        prev = current;
        current = current->next;
    }

    return strdup("Rule not found");
}

char *handleListRules(){
    char *response = malloc(4096);
    response[0] = '\0';

    char line[128];

    Rule *current = rules;

    while(current != NULL){
        sprintf(line, "Rule: %s\n", current->rule_string);
        strcat(response, line);
        Query *q = current->queries;
        while(q!=NULL){
            sprintf(line, "Query: %s %d\n", q->ip, q->port);
            strcat(response,line);
            q = q->next;

        }
        current = current->next;


    }





    return response;
}

char *handleFree(){
    Rule *current = rules;
    while(current != NULL){
        Query *q = current->queries;
        while(q != NULL){
            Query *q_temp = q;
            q = q->next;
            free(q_temp);

        }
        Rule *nextRule = current->next;
        free(current);
        current = nextRule;

    }
    rules = NULL;

    Request *r = requests;
    while(r!=NULL){
        Request *temp = r;
        r = r->next;
        free(temp);
    }
    requests = NULL;
    




    return strdup("All rules deleted");
}

extern char *processRequest (char *);

char *processRequest (char *request) {
    pthread_mutex_lock(&lock);

    // store history
    addRequest(request);
    char *response = NULL;

    switch(request[0]){
        case 'R':
            response = handleListRequests();
            break;
        case 'A':
            response = handleAdd(request + 2);
            break;
        case 'C':
            response = handleCheck(request + 2);
            break;
        case 'D':
            response = handleDelete(request + 2);
            break;
        case 'L':
            response = handleListRules();
            break;
        case 'F':
            response = handleFree();
            break;
        default:
            response = strdup("Illegal request"); 
    }

    pthread_mutex_unlock(&lock);
    return response;
}


