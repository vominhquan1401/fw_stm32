SET PATH=C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin;C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeMX\;C:\Program Files\Common Files\Oracle\Java\javapath;C:\Program Files\Microsoft\jdk-11.0.12.7-hotspot\bin;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files\NVIDIA Corporation\NVIDIA NvDLISR;C:\Program Files\dotnet\;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;C:\Program Files\Git\cmd;C:\Program Files\Cloudflare\Cloudflare WARP\;D:\lab\CANable\apache-ant-1.10.15-bin\apache-ant-1.10.15\bin;;C:\ProgramData\chocolatey\bin;C:\Program Files\Docker\Docker\resources\bin;C:\Program Files\nodejs\;C:\Users\Admin\AppData\Local\Programs\Python\Python311\Scripts\;C:\Users\Admin\AppData\Local\Programs\Python\Python311\;C:\Users\Admin\AppData\Local\Microsoft\WindowsApps;C:\Users\Admin\AppData\Local\Programs\Microsoft VS Code\bin;C:\Users\Admin\.dotnet\tools;C:\Users\Admin\AppData\Local\GitHubDesktop\bin;C:\Program Files\Java\jdk-18.0.2.1\bin;C:\Program Files\JetBrains\IntelliJ IDEA Community Edition 2022.3\bin;;C:\msys64\ucrt64\bin;C:\MinGW\bin;C:\Users\Admin\AppData\Roaming\npm;C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeMX\.\jre\\bin
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=fw_stm32
SET CPU_CLOCK=0x007A1200
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmAsm" --Via ".\fw_stm32\startup_stm32f103xb._ia"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\main.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_it.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal_msp.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal_gpio_ex.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal_tim.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal_tim_ex.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal_rcc.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal_rcc_ex.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal_gpio.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal_dma.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal_cortex.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal_pwr.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal_flash.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal_flash_ex.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\stm32f1xx_hal_exti.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via ".\fw_stm32\system_stm32f1xx.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmLink" --Via ".\fw_stm32\fw_stm32.lnp"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\fromelf.exe" ".\fw_stm32\fw_stm32.axf" --i32combined --output ".\fw_stm32\fw_stm32.hex"
fromelf --bin --output=.\fw_stm32\fw_stm32.bin .\fw_stm32\fw_stm32.axf
