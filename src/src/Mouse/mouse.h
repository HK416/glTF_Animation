/************************/
/*  FILE NAME: mouse.h  */
/************************/
#ifndef _MOUSE_H_
#define _MOUSE_H_

/************************/
/*  STRUCT NAME: Mouse  */
/************************/
struct Mouse
{
    double x_pos = 0.0;
    double y_pos = 0.0;
    double sensitivity = 1.0;
    bool first_input = true;
}; // struct Mouse
#endif // !_MOUSE_H_