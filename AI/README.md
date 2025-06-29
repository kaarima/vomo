# Voice-Controlled Robot API Documentation

This document describes a FastAPI application that processes voice commands to control a robot's movements and actions using audio transcription.

## Overview

The application records audio, transcribes it using the Groq API with the Whisper-large-v3 model, and interprets commands to control a robot's movement (forward, backward, left, right), stop, scan, or perform a special action. The commands may include numeric values for distance or rotation angles.

## Dependencies

- **FastAPI**: Web framework for building the API.
- **sounddevice**: Library for recording audio.
- **numpy**: Used for handling audio data.
- **scipy.io.wavfile**: For saving audio as WAV files.
- **groq**: Groq API client for audio transcription.
- **re**: Regular expression module for parsing numeric values from text.
- **os**: For file path handling.

## Configuration

- **Audio Settings**:
  - `duration = 2`: Recording duration in seconds.
  - `fs = 16000`: Audio sample rate (16 kHz).
  - Audio is recorded in mono (1 channel) with 16-bit integer format.
- **API Key**:
  - `api_key`: Hardcoded Groq API key for transcription (replace with environment variable in production for security).
- **File Path**:
  - `filename`: Path to save the recorded WAV file (`recorded.wav` in the script's directory).

## Endpoints

### GET `/`

**Description**: Records a 2-second audio clip, transcribes it, and processes the transcription to extract robot control commands.

**Process**:
1. **Record Audio**:
   - Uses `sounddevice.rec` to capture audio for the specified duration and sample rate.
   - Saves the audio as a WAV file using `scipy.io.wavfile.write`.
2. **Transcribe Audio**:
   - Sends the WAV file to the Groq API for transcription using the `whisper-large-v3` model.
   - Receives a verbose JSON response with the transcribed text.
3. **Parse Commands**:
   - Analyzes the transcribed text (case-insensitive) for specific keywords:
     - `"stop"`: Sets `stop` flag to `True` and prints "Stopping the robot".
     - `"scan"`: Sets `scan` flag to `True` and prints "Scanning...".
     - `"special"`: Sets `special` flag to `True` and prints "Special...".
     - `"forward"`: Extracts a float value (e.g., "3.5") using regex and assigns it to `value`. Prints "Moving forward by {value} cm".
     - `"backward"`: Extracts a float value, negates it, and assigns it to `value`. Prints "Moving backward by {value} cm".
     - `"left"`: Extracts a float value and assigns it to `degree`. Prints "Turning left by {value} degrees".
     - `"right"`: Extracts a float value, negates it, and assigns it to `degree`. Prints "Turning right by {value} degrees".
     - If no valid command is found, prints "No valid command found in the transcription".
   - Uses regex pattern `r'\d+(\.\d+)?'` to extract float values (e.g., "3.5" or "10").
4. **Response**:
   - Returns a JSON response with the following fields:
     - `distance`: Float value for forward/backward movement (positive for forward, negative for backward, 0.0 if not applicable).
     - `rotation`: Float value for left/right rotation (positive for left, negative for right, 0.0 if not applicable).
     - `stop`: Boolean indicating if the stop command was detected.
     - `scan`: Boolean indicating if the scan command was detected.
     - `special`: Boolean indicating if the special command was detected.

**Example Response**:
```json
{
  "distance": 5.0,
  "rotation": 0.0,
  "stop": false,
  "scan": false,
  "special": false
}
```

**Error Handling**:
- If no numeric value is found for "forward", "backward", "left", or "right" commands, prints a warning and sets the respective value to 0.0.
- No explicit error handling for file I/O or API failures is implemented (consider adding try-except blocks for production).

## Usage Example

1. **Start the FastAPI server**:
   ```bash
   uvicorn main:app --reload
   ```
2. **Send a GET request** to `http://localhost:8000/`:
   - The server records 2 seconds of audio.
   - If you say "Move forward 5.0", the response might be:
     ```json
     {
       "distance": 5.0,
       "rotation": 0.0,
       "stop": false,
       "scan": false,
       "special": false
     }
     ```
   - Console output: `Moving forward by 5.0 cm`.

## Notes

- **Security**: The API key is hardcoded, which is insecure. Use environment variables (e.g., `os.getenv`) in production.
- **Error Handling**: Add try-except blocks for file operations and API calls to handle potential failures (e.g., file not found, API timeout).
- **Regex Limitation**: The regex `r'\d+(\.\d+)?'` matches integers and simple floats (e.g., "3.5") but may fail for complex numbers or malformed input. Consider enhancing the regex for robustness.
- **Variable Naming**: The variable `value` is used for both distance and degree in console messages, which could cause confusion (e.g., "Turning left by {value} degrees" should use `degree`).
- **Asynchronous Execution**: The endpoint uses `async def`, which is appropriate for FastAPI, but ensure the Groq client supports async operations.
- **File Management**: The WAV file is overwritten on each request. Consider unique filenames or cleanup to avoid conflicts in concurrent requests.

## Improvements

- Add input validation for audio duration and sample rate.
- Implement error handling for transcription failures or invalid audio files.
- Support more complex voice commands (e.g., "move forward 5 meters and turn left 90 degrees").
- Use environment variables for sensitive data like API keys.
- Add logging instead of print statements for better monitoring.
- Consider streaming audio to avoid saving files, if supported by the Groq API.
