import QtQuick 2.15

Rectangle {
  id: speedometer
  width: 250
  height: 240
  color: "transparent"

  property int speed: vehicleData.speed
  property int maxSpeed: 160
  property real needleAngle: (speed / maxSpeed) * 270 - 135 // Start from -135 degrees (vertical top) for proper 0 alignment

  Canvas {
  id: speedometerCanvas
  anchors.fill: parent
        
  onPaint: {
    var ctx = getContext("2d")
    var centerX = width / 2
    var centerY = height / 2
    var radius = Math.min(centerX, centerY) - 20
            
    // Clear canvas
    ctx.clearRect(0, 0, width, height)
            
    // Draw outer circle
    ctx.beginPath()
    ctx.arc(centerX, centerY, radius, 0, 2 * Math.PI)
    ctx.strokeStyle = "#333"
    ctx.lineWidth = 3
    ctx.stroke()
            
    // Draw speed markings
    ctx.strokeStyle = "#666"
    ctx.lineWidth = 2
    ctx.font = "bold 16px sans-serif"
    ctx.fillStyle = "#333"
    ctx.textAlign = "center"
            
    for (var i = 0; i <= maxSpeed; i += 20) {
      var angle = (i / maxSpeed) * 270 - 135
      var radian = angle * Math.PI / 180
      var x1 = centerX + (radius - 15) * Math.cos(radian)
      var y1 = centerY + (radius - 15) * Math.sin(radian)
      var x2 = centerX + radius * Math.cos(radian)
      var y2 = centerY + radius * Math.sin(radian)
                
      ctx.beginPath()
      ctx.moveTo(x1, y1)
      ctx.lineTo(x2, y2)
      ctx.stroke()
                
      // Add numbers
      if (i % 40 === 0) {
        var textX = centerX + (radius - 30) * Math.cos(radian)
        var textY = centerY + (radius - 30) * Math.sin(radian) + 4
        ctx.fillText(i.toString(), textX, textY)
      }
    }
            
    // Draw center circle
    ctx.beginPath()
    ctx.arc(centerX, centerY, 8, 0, 2 * Math.PI)
    ctx.fillStyle = "#333"
    ctx.fill()
  }
        
  // Redraw when speed changes
  Connections {
  target: vehicleData
  function onSpeedChanged() {
    speedometerCanvas.requestPaint()
  }
}
}
    
  // Speedometer needle
  Rectangle {
  id: needle
  width: 4
  height: speedometer.height * 0.35
  color: "#ff4444"
  anchors.horizontalCenter: parent.horizontalCenter
  anchors.bottom: parent.verticalCenter
  transformOrigin: Item.Bottom
  rotation: needleAngle
        
  Behavior on rotation {
  SmoothedAnimation { duration: 300 }
}
}
    
  // Speed label
  Text {
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    anchors.verticalCenterOffset: 10
    text: "km/h"
    color: "#F54927"
    font.pixelSize: 16
    font.bold: true
    font.family: "sans-serif"
  }
  
  // Digital speed display
  Rectangle {
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 40
    width: 80
    height: 30
    color: "#1a1a1a"
    border.color: "#333"
    border.width: 1
    radius: 4
        
    Text {
      anchors.centerIn: parent
      text: speed.toString()
      color: "#00ff00"
      font.pixelSize: 16
      font.bold: true
      font.family: "monospace"
    }
  }
}
