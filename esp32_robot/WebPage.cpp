#include "WebPage.h"

String getWebPage() {
  return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Advanced Robot Control Dashboard</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            margin: 0;
            padding: 20px;
            min-height: 100vh;
            display: flex;
            flex-direction: column;
            align-items: center;
        }
        .container {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 20px;
            padding: 30px;
            box-shadow: 0 15px 35px rgba(0, 0, 0, 0.2);
            text-align: center;
            max-width: 500px;
            width: 100%;
            margin-bottom: 20px;
        }
        h1 {
            color: #333;
            margin-bottom: 30px;
            font-size: 28px;
        }
        h2 {
            color: #555;
            margin: 25px 0 15px 0;
            font-size: 20px;
            border-bottom: 2px solid #ddd;
            padding-bottom: 10px;
        }
        .control-grid {
            display: grid;
            grid-template-columns: 1fr 1fr 1fr;
            gap: 15px;
            margin: 20px 0;
        }
        .btn {
            padding: 15px 20px;
            border: none;
            border-radius: 10px;
            font-size: 16px;
            font-weight: bold;
            cursor: pointer;
            transition: all 0.3s ease;
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
        }
        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 6px 20px rgba(0, 0, 0, 0.3);
        }
        .btn:active {
            transform: translateY(0);
        }
        .forward { background: #4CAF50; color: white; }
        .backward { background: #f44336; color: white; }
        .left { background: #FF9800; color: white; }
        .right { background: #2196F3; color: white; }
        .stop { background: #9E9E9E; color: white; grid-column: 2; }
        .led-btn { background: #9C27B0; color: white; }
        .voice-btn { background: #E91E63; color: white; }
        .calibrate-btn { background: #00BCD4; color: white; }
        .special-btn { background: #FF5722; color: white; }
        .scan-btn { background: #795548; color: white; }
        
        .full-width {
            width: 100%;
            margin: 10px 0;
        }
        
        .calibration-section {
            background: #f8f9fa;
            padding: 20px;
            border-radius: 10px;
            margin: 20px 0;
        }
        
        .input-group {
            margin: 15px 0;
            text-align: left;
        }
        
        .input-group label {
            display: block;
            margin-bottom: 5px;
            color: #555;
            font-weight: bold;
        }
        
        .input-group input {
            width: 100%;
            padding: 10px;
            border: 2px solid #ddd;
            border-radius: 5px;
            font-size: 16px;
        }
        
        .status {
            margin-top: 20px;
            padding: 15px;
            border-radius: 8px;
            background: #e3f2fd;
            color: #1565c0;
            font-weight: bold;
        }
        
        .calibration-values {
            background: #fff3e0;
            padding: 15px;
            border-radius: 8px;
            margin: 15px 0;
        }
        
        .empty { visibility: hidden; }
        
        .recording {
            background: #ff1744 !important;
            animation: pulse 1s infinite;
        }
        
        @keyframes pulse {
            0% { opacity: 1; }
            50% { opacity: 0.7; }
            100% { opacity: 1; }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🤖 Advanced Robot Control</h1>
        
        <h2>Manual Control</h2>
        <div class="control-grid">
            <div class="empty"></div>
            <button class="btn forward" onclick="sendCommand('forward')">⬆️ Forward</button>
            <div class="empty"></div>
            
            <button class="btn left" onclick="sendCommand('left')">⬅️ Left</button>
            <button class="btn stop" onclick="sendCommand('stop')">⏹️ Stop</button>
            <button class="btn right" onclick="sendCommand('right')">➡️ Right</button>
            
            <div class="empty"></div>
            <button class="btn backward" onclick="sendCommand('backward')">⬇️ Backward</button>
            <div class="empty"></div>
        </div>
        
        <h2>Advanced Features</h2>
        <button class="btn voice-btn full-width" id="voiceBtn" onclick="toggleVoiceRecord()">🎤 Voice Command</button>
        <button class="btn special-btn full-width" onclick="sendCommand('special')">✨ Special Sequence</button>
        <button class="btn scan-btn full-width" onclick="sendCommand('scan')">🔍 Scan Area</button>
        <button class="btn led-btn full-width" onclick="toggleLED()">💡 Toggle LED</button>
        
        <div class="calibration-section">
            <h2>Calibration</h2>
            <div class="calibration-values">
                <strong>Current Settings:</strong><br>
                Distance: <span id="distanceVal">10.0</span> cm/sec<br>
                Rotation: <span id="rotationVal">90.0</span> deg/sec
            </div>
            
            <button class="btn calibrate-btn full-width" onclick="startDistanceCalibration()">📏 Calibrate Distance</button>
            <button class="btn calibrate-btn full-width" onclick="startRotationCalibration()">🔄 Calibrate Rotation</button>
            
            <div id="calibrationInputs" style="display: none;">
                <div class="input-group">
                    <label for="actualValue">Actual Value:</label>
                    <input type="number" id="actualValue" placeholder="Enter measured value">
                </div>
                <button class="btn calibrate-btn" onclick="submitCalibration()">✅ Submit</button>
                <button class="btn" onclick="cancelCalibration()">❌ Cancel</button>
            </div>
        </div>
        
        <div class="status" id="status">Ready to control robot</div>
    </div>

    <script>
        let isRecording = false;
        let calibrationType = '';
        
        function sendCommand(command) {
            fetch('/' + command)
                .then(response => response.text())
                .then(data => {
                    document.getElementById('status').innerHTML = '✅ ' + data;
                })
                .catch(error => {
                    document.getElementById('status').innerHTML = '❌ Error: ' + error;
                });
        }
        
        function toggleLED() {
            fetch('/led')
                .then(response => response.text())
                .then(data => {
                    document.getElementById('status').innerHTML = '💡 ' + data;
                })
                .catch(error => {
                    document.getElementById('status').innerHTML = '❌ Error: ' + error;
                });
        }
        
        function toggleVoiceRecord() {
            const btn = document.getElementById('voiceBtn');
            if (!isRecording) {
                isRecording = true;
                btn.classList.add('recording');
                btn.innerHTML = '🔴 Recording...';
                document.getElementById('status').innerHTML = '🎤 Listening for voice command...';
                
                // Simulate recording for 3 seconds, then send request
                setTimeout(() => {
                    fetch('/voice')
                        .then(response => response.text())
                        .then(data => {
                            document.getElementById('status').innerHTML = '🎤 ' + data;
                        })
                        .catch(error => {
                            document.getElementById('status').innerHTML = '❌ Voice Error: ' + error;
                        })
                        .finally(() => {
                            isRecording = false;
                            btn.classList.remove('recording');
                            btn.innerHTML = '🎤 Voice Command';
                        });
                }, 10);
            }
        }
        
        function startDistanceCalibration() {
            calibrationType = 'distance';
            document.getElementById('calibrationInputs').style.display = 'block';
            document.getElementById('actualValue').placeholder = 'Distance in cm';
            sendCommand('calibrate_distance');
        }
        
        function startRotationCalibration() {
            calibrationType = 'rotation';
            document.getElementById('calibrationInputs').style.display = 'block';
            document.getElementById('actualValue').placeholder = 'Angle in degrees';
            sendCommand('calibrate_rotation');
        }
        
        function submitCalibration() {
            const actualValue = document.getElementById('actualValue').value;
            if (actualValue) {
                fetch('/calibrate_submit?type=' + calibrationType + '&value=' + actualValue)
                    .then(response => response.text())
                    .then(data => {
                        document.getElementById('status').innerHTML = '✅ ' + data;
                        updateCalibrationDisplay();
                        cancelCalibration();
                    })
                    .catch(error => {
                        document.getElementById('status').innerHTML = '❌ Calibration Error: ' + error;
                    });
            }
        }
        
        function cancelCalibration() {
            document.getElementById('calibrationInputs').style.display = 'none';
            document.getElementById('actualValue').value = '';
            calibrationType = '';
        }
        
        function updateCalibrationDisplay() {
            fetch('/get_calibration')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('distanceVal').textContent = data.distance.toFixed(1);
                    document.getElementById('rotationVal').textContent = data.rotation.toFixed(1);
                })
                .catch(error => {
                    console.error('Error updating calibration display:', error);
                });
        }
        
        // Keyboard controls
        document.addEventListener('keydown', function(event) {
            if (document.activeElement.tagName === 'INPUT') return;
            
            switch(event.key) {
                case 'ArrowUp':
                case 'w':
                case 'W':
                    sendCommand('forward');
                    break;
                case 'ArrowDown':
                case 's':
                case 'S':
                    sendCommand('backward');
                    break;
                case 'ArrowLeft':
                case 'a':
                case 'A':
                    sendCommand('left');
                    break;
                case 'ArrowRight':
                case 'd':
                case 'D':
                    sendCommand('right');
                    break;
                case ' ':
                    event.preventDefault();
                    sendCommand('stop');
                    break;
                case 'v':
                case 'V':
                    toggleVoiceRecord();
                    break;
            }
        });
        
        // Load calibration values on page load
        updateCalibrationDisplay();
    </script>
</body>
</html>
  )rawliteral";
}
