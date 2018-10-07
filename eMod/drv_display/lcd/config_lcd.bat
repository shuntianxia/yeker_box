@echo off
	goto START

:START
    @echo *********************************************
    @echo *   select board                            *
    @echo *********************************************
    @echo  0: InnoLux_at043
    @echo  1: R61509_cpupanel_16bit
    @echo *********************************************
    
    set /p SEL=Please Select:
    if %SEL%==0     goto LCD0
    if %SEL%==1     goto LCD1
    goto ERROR

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LCD0
    copy lcd_bak\InnoLux_at043.c lcd_panel_cfg.c
    goto conti
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LCD1
    copy lcd_bak\R61509.c lcd_panel_cfg.c
    goto conti

:ERROR
    @echo sel error!!
    goto conti
    
:conti
    @echo *********************************************
    pause

