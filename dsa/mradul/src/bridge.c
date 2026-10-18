#include "linked_list.h"
#include "queue.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>

static void clean(char *s) { s[strcspn(s, "\r\n")] = '\0'; }
static void json_string(const char *s) {
    putchar('"');
    for (; *s; s++) {
        if (*s == '"' || *s == '\\') { putchar('\\'); putchar(*s); }
        else if (*s == '\n') printf("\\n");
        else if ((unsigned char)*s >= 32) putchar(*s);
    }
    putchar('"');
}
int main(int argc, char **argv) {
    char line[256];
    if (argc < 2) { fprintf(stderr, "Usage: dsa_bridge search|list|queue|stack\n"); return 2; }
    if (strcmp(argv[1], "search") == 0 || strcmp(argv[1], "list") == 0) {
        ActiveOccupancyList list; init_active_list(&list);
        while (fgets(line, sizeof line, stdin)) {
            clean(line); if (!line[0]) continue;
            char *tab = strchr(line, '\t'); if (!tab) continue; *tab++ = '\0';
            insert_active_vehicle(&list, line, atoi(tab));
        }
        if (strcmp(argv[1], "search") == 0) {
            if (argc < 3) { free_active_list(&list); return 2; }
            Node *found = search_active_vehicle(&list, argv[2]);
            if (!found) puts("{\"found\":false}");
            else { printf("{\"found\":true,\"vehicleNumber\":"); json_string(found->record.vehicle_number); printf(",\"slotId\":%d}\n", found->record.slot_id); }
        } else {
            printf("{\"activeCount\":%d,\"vehicles\":[", list.active_count);
            Node *n = list.head; int first = 1;
            while (n) { if (!first) putchar(','); first=0; printf("{\"vehicleNumber\":"); json_string(n->record.vehicle_number); printf(",\"slotId\":%d}", n->record.slot_id); n=n->next; }
            puts("]}");
        }
        free_active_list(&list); return 0;
    }
    if (strcmp(argv[1], "queue") == 0) {
        ReservationQueue q; init_queue(&q);
        while (fgets(line, sizeof line, stdin)) {
            clean(line); if (!line[0]) continue;
            char *a=strchr(line,'\t'); if(!a) continue; *a++='\0';
            char *b=strchr(a,'\t'); if(!b) continue; *b++='\0';
            Reservation r; memset(&r,0,sizeof r);
            snprintf(r.vehicle_number,sizeof r.vehicle_number,"%.15s",line);
            snprintf(r.reservation_time,sizeof r.reservation_time,"%s",a);
            r.slot_type=atoi(b);
            if (!enqueue_reservation(&q,r)) { fprintf(stderr,"Reservation queue capacity (50) exceeded.\n"); return 3; }
        }
        printf("{\"count\":%d,\"reservations\":[",q.count);
        for(int i=0;i<q.count;i++){ int idx=(q.front+i)%QUEUE_CAPACITY; if(i)putchar(','); printf("{\"vehicleNumber\":");json_string(q.items[idx].vehicle_number);printf(",\"reservationTime\":");json_string(q.items[idx].reservation_time);printf(",\"slotType\":%d}",q.items[idx].slot_type); }
        puts("]}"); return 0;
    }
    if (strcmp(argv[1], "stack") == 0) {
        UndoStack s; init_stack(&s);
        while(fgets(line,sizeof line,stdin)){
            clean(line); if(!line[0])continue;
            char *a=strchr(line,'\t'); if(!a)continue; *a++='\0';
            char *b=strchr(a,'\t'); if(!b)continue; *b++='\0';
            GateAction action; memset(&action,0,sizeof action);
            snprintf(action.action_type,sizeof action.action_type,"%.15s",line);
            snprintf(action.vehicle_number,sizeof action.vehicle_number,"%s",a);
            action.slot_id=atoi(b);
            if(!push_action(&s,action)){fprintf(stderr,"Action stack capacity (50) exceeded.\n");return 3;}
        }
        printf("{\"count\":%d,\"actions\":[",s.top+1);
        for(int i=s.top;i>=0;i--){if(i!=s.top)putchar(',');printf("{\"action\":");json_string(s.actions[i].action_type);printf(",\"vehicleNumber\":");json_string(s.actions[i].vehicle_number);printf(",\"slotId\":%d}",s.actions[i].slot_id);}
        puts("]}");return 0;
    }
    fprintf(stderr,"Unknown command.\n"); return 2;
}
