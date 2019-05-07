

typedef enum {
    STATE1 = 0, STATE2, MAX_STATE
} estados;
typedef enum {
    INPUT1 = 0, INPUT2, MAX_INPUT
} entradas;
typedef void (*cb_t)(void);

cb_t machine[MAX_STATE][MAX_INPUT] = {
    0, 0,  /* funções para estado 1 */
    0, 0   /* funções para estado 2 */
};
estados next_state[MAX_STATE][MAX_INPUT] = {
    0, 0,  /* próximos estados para estado 1 */
    0, 0   /* prósimos estados para estado 2 */
};
estados state  = STATE1;



//

/teste

teste//
teste/


int main()
{
    entradas input;

    while(1) {
        input = wait_for_events();
        machine[state][input]; /* mais testes*/
        state = next_state[estado];
    }

    return 0;
}
