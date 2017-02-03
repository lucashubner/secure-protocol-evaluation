json.extract! sensor_read, :id, :value, :sensor_type_id, :created_at, :updated_at
json.url sensor_read_url(sensor_read, format: :json)