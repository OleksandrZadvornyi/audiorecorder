# Enhanced Qt Audio Recorder

This project is an enhanced version of the Qt Audio Recorder example, modified for educational purposes to demonstrate various design patterns in C++/Qt application development.

## Project Overview

The original Qt Audio Recorder example provides a simple GUI application for recording audio using Qt's multimedia capabilities. This project extends that foundation by implementing several design patterns to demonstrate software engineering principles in a practical context.

## Design Patterns Implemented

This project implements the following design patterns:

1. **Observer Pattern**: Implemented through `Observer` and `Subject` interfaces to allow various components to monitor recording state changes.

   - `StatusObserver`: Updates the UI status bar with recording state
   - `RecordingTimeDisplay`: Updates the recording time display
   - `LogFileObserver`: Logs recording events to a file

2. **State Pattern**: Manages recording state transitions with:

   - `RecordingState`: Behavior when recording
   - `PausedState`: Behavior when paused
   - `StoppedState`: Behavior when stopped

3. **Command Pattern**: Encapsulates recording operations as objects:

   - `StartRecordingCommand`: Initiates recording
   - `PauseRecordingCommand`: Pauses recording
   - `ResumeRecordingCommand`: Resumes recording
   - `StopRecordingCommand`: Stops recording
   - Includes a command history feature visible in the UI

4. **Strategy Pattern**: Allows different audio encoding approaches:

   - `ConstantQualityStrategy`: Prioritizes quality over file size
   - `ConstantBitRateStrategy`: Maintains consistent bit rate

5. **Facade Pattern**: Simplifies access to the recording subsystem:
   - `RecordingFacade`: Provides a unified interface to the various recording components

## Getting Started

### Prerequisites

- Qt 6.0 or higher
- C++17 compatible compiler

### Building the Project

1. Open the project file `audiorecorder.pro` in Qt Creator
2. Configure the project for your development environment
3. Build and run the application

## License

This project is distributed under the BSD 3-Clause License. See the [LICENSE](LICENSE) file for details.

The original Qt example code is Copyright (C) 2017 The Qt Company Ltd. and is used under the BSD 3-Clause License.

## Acknowledgments

- Qt Company for providing the original audio recorder example
- This project is for educational purposes only
