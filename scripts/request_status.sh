message="Lukas-Desktop"
topic="CControl/Status/Request"
host="192.168.2.150"

mosquitto_pub -t $topic -h $host -m $message
