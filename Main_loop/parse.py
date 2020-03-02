import json
def parse(bstring, value_labels):
	decoded = bstring.decode('utf-8')
	splitted = decoded.split(':')
	data_type, values = splitted[0], splitted[1]
	values = values.rstrip('\r\n').split(',')
	values_dict = {label: float(vals) for label, vals in zip(value_labels, values)}
	final_dict = {"data_type": data_type, "values": values_dict}
	json_str = json.dumps(final_dict)
	final_json = json.loads(json_str)
	return final_json