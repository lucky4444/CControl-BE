message="{\"clientid\":\"Lukas-Desktop\",\"action\":\"prev\"}"
topic="CControl/Play"
host="192.168.2.150"

mosquitto_pub -t $topic -h $host -m $message
