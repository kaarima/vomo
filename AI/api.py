from fastapi import FastAPI
from fastapi.responses import JSONResponse
import os
from groq import Groq
import re
import sounddevice as sd
import numpy as np
import scipy.io.wavfile as wav

duration = 2  # seconds
fs = 16000    # sample rate

# initialization
app = FastAPI()


api_key = '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'
client = Groq(api_key=api_key)
filename = os.path.dirname(__file__) + "/recorded.wav"


@app.get("/")
async def move():
    print("Recording...")
    audio = sd.rec(int(duration * fs), samplerate=fs, channels=1, dtype='int16')
    sd.wait()
    print("Recording complete.")

    wav.write("recorded.wav", fs, audio)
    with open(filename, "rb") as file:
        transcription = client.audio.transcriptions.create(
          file=(filename, file.read()),
          model="whisper-large-v3",
          response_format="verbose_json",
        )
        result = transcription.text
        value = 0.0
        degree = 0.0
        stop = False
        scan = False
        special = False
        if "stop" in result.lower():
            stop = True
            print("Stopping the robot")
        elif "scan" in result.lower():
            scan = True
            print("Scanning...")
        elif "special" in result.lower():
            special = True
            print("Special...")
        elif "forward" in result.lower():
            # find the float value in the transcription
            match = re.search(r'\d+(\.\d+)?', result)
            if match:
                value = float(match.group())
                print(f"Moving forward by {value} cm")
            else:
                print("No numeric value found to move forward")
        elif "backward" in result.lower():
            # find the float value in the transcription
            match = re.search(r'\d+(\.\d+)?', result)
            value = - float(match.group())
            print(f"Moving backward by {value} cm")
        elif "left" in result.lower():
            match = re.search(r'\d+(\.\d+)?', result)
            degree = float(match.group())
            print(f"Turning left by {value} degrees")
        elif "right" in result.lower():
            match = re.search(r'\d+(\.\d+)?', result)
            degree = - float(match.group())
            print(f"Turning right by {value} degrees")
        else:
            print("No valid command found in the transcription")
    
    print(f"Transcription result:", {"distance": value, 'rotation': degree, 'stop': stop, 'scan': scan, 'special': special})
    return JSONResponse(content={"distance": value, 'rotation': degree, 'stop': stop, 'scan': scan, 'special': special})
