message="{\"clientid\":\"Lukas-Desktop\",\"action\":\"set\",\"time\":$1}"
topic="CControl/Shutdown"
host="192.168.2.150"

mosquitto_pub -t $topic -h $host -m $message
