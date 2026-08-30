// Basic Calculator
typedef enum
{
    CALC_NONE,
    CALC_ADD,
    CALC_SUB,
    CALC_MUL,
    CALC_DIV,
    CALC_EQUAL,
} Calculator_Operation_Kind;

internal Calculator_Operation_Kind operation = CALC_NONE; 
internal Calculator_Operation_Kind last_op = CALC_NONE; 

internal int runningTotal = 0;
internal int input = 0;

void render_calculator(Bui* bui)
{
    char calculatorScreenBuf[256];
    switch (operation)
    {
        case CALC_NONE:
        case CALC_EQUAL:
            {
                sprintf(calculatorScreenBuf, "%d", runningTotal);
            } break;
        case CALC_ADD:
        case CALC_SUB:
        case CALC_MUL:
        case CALC_DIV:
            {
                sprintf(calculatorScreenBuf, "%d", input);
            } break;
    }

    // bui_text(bui, "");

    bui_begin_window(bui, "Main", 100, 100, 200, 200);

    bui_text(bui, calculatorScreenBuf);

    if (bui_button(bui, "7"))
    {
        if (operation == CALC_NONE)
        {
            runningTotal = runningTotal * 10 + 7;
        }
        else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
        {
            input = input * 10 + 7;
        }
    }

    bui_same_line(bui);

    if (bui_button(bui, "8"))
    {
        if (operation == CALC_NONE)
        {
            runningTotal = runningTotal * 10 + 8;
        }
        else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
        {
            input = input * 10 + 8;
        }
    }

    bui_same_line(bui);

    if (bui_button(bui, "9"))
    {
        if (operation == CALC_NONE)
        {
            runningTotal = runningTotal * 10 + 9;
        }
        else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
        {
            input = input * 10 + 9;
        }
    }

    bui_same_line(bui);

    if (bui_button(bui, "+"))
    {
        operation = CALC_ADD;
        input = 0;
    }

    if (bui_button(bui, "4"))
    {
        if (operation == CALC_NONE)
        {
            runningTotal = runningTotal * 10 + 4;
        }
        else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
        {
            input = input * 10 + 4;
        }
    }

    bui_same_line(bui);

    if (bui_button(bui, "5"))
    {
        if (operation == CALC_NONE)
        {
            runningTotal = runningTotal * 10 + 5;
        }
        else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
        {
            input = input * 10 + 5;
        }
    }

    bui_same_line(bui);

    if (bui_button(bui, "6"))
    {
        if (operation == CALC_NONE)
        {
            runningTotal = runningTotal * 10 + 6;
        }
        else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
        {
            input = input * 10 + 6;
        }
    }

    bui_same_line(bui);

    if (bui_button(bui, "-"))
    {
        operation = CALC_SUB;
        input = 0;
    }

    if (bui_button(bui, "1"))
    {
        if (operation == CALC_NONE)
        {
            runningTotal = runningTotal * 10 + 1;
        }
        else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
        {
            input = input * 10 + 1;
        }
    }

    bui_same_line(bui);

    if (bui_button(bui, "2"))
    {
        if (operation == CALC_NONE)
        {
            runningTotal = runningTotal * 10 + 2;
        }
        else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
        {
            input = input * 10 + 2;
        }
    }

    bui_same_line(bui);

    if (bui_button(bui, "3"))
    {
        if (operation == CALC_NONE)
        {
            runningTotal = runningTotal * 10 + 3;
        }
        else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
        {
            input = input * 10 + 3;
        }
    }

    bui_same_line(bui);

    if (bui_button(bui, "*"))
    {
        operation = CALC_MUL;
        input = 0;
    }

    if (bui_button(bui, "C"))
    {
        runningTotal = 0;
        input = 0;
        operation = CALC_NONE;
    }

    bui_same_line(bui);

    if (bui_button(bui, "0"))
    {
        if (operation == CALC_NONE)
        {
            runningTotal = runningTotal * 10 + 0;
        }
        else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
        {
            input = input * 10 + 0;
        }
    }

    bui_same_line(bui);

    if (bui_button(bui, "="))
    {
        if (operation == CALC_EQUAL)
        {
            operation = last_op;
        }

        if (operation == CALC_ADD)
        {
            runningTotal += input;
        }

        if (operation == CALC_SUB)
        {
            runningTotal -= input;
        }

        if (operation == CALC_MUL)
        {
            runningTotal *= input;
        }

        if (operation == CALC_DIV)
        {
            if (runningTotal != 0 && input != 0)
            {
                runningTotal /= input;
            }
        }

        last_op = operation;
        operation = CALC_EQUAL;
    }

    bui_same_line(bui);

    if (bui_button(bui, "/"))
    {
        operation = CALC_DIV;
        input = 0;
    }

    bui_text(bui, "  ");
    bui_same_line(bui);

    if (bui_button(bui, " <= "))
    {
        if (operation == CALC_NONE || operation == CALC_EQUAL)
        {
            if (runningTotal > 0)
            {
                runningTotal = runningTotal / 10;
            }
        }
        else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
        {
            if (input > 0)
            {
                input = input / 10;
            }
        }
    }
    bui_end_window(bui);
}
