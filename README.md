# breathere-interface
User interface for the breathere emergency pandemic ventilator https://www.breathere.ca/

To ensure safe device operation and reliability, the ventilator can be fully operated without the user interface with simple electromechanical components. However, to facilitate the patient monitoring, a cross-platform application is provided to run on an android tablet, iOS device, Windows, Mac or regular/embedded Linux. This allows manufacturing the device rapidly with any computing platform that is rapidly available in volume to the manufacturer. To compile and run the software:
1.	Download the Qt framework (qt.io/download).
2.	During the installation process, select the appropriate frameworks (PC, iOS, Android). 
3.	For a mobile device, download the appropriate toolchain.
        Android : https://developer.android.com/ndk
        iOS : https://developer.apple.com/xcode/
4.	Download the source code from the github repository 
        Github: https://github.com/lemwill/breathere-interface.git).
5.	Open the breathere-interface.pro project file from the Qt Creator IDE.
6.	Select the target build platform (Desktop, iOS or Android).
7.	Execute the software (Figure 4).
