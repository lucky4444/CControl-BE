message="{\"clientid\":\"Lukas-Desktop\",\"volume\":$1}"
topic="CControl/Volume"
host="192.168.2.150"

mosquitto_pub -t $topic -h $host -m $message
