# ESP32 Calculator: Version 1

ESP32 Core Version Needed: 3.3.12
> This code seems to work on versions below 3.3.12, however development was done on version 3.3.12.

**Support and Limitations**
This version supports the following:
> 4 basic operators (add, subtract, multiply, divide)
> Decimals
> Negative Numbers
> Parenthesis
    - Note: Parenthesis containing *only one number* will fail to be recognized by the calculator
    - This means that writing out 53.12+(43) will only return 53.12.
    - This also means that singular negative numbers will have to be written without parenthesis. EX: 4+-4.
