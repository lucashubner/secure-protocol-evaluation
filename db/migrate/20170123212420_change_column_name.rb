class ChangeColumnName < ActiveRecord::Migration[5.0]
  def change
	rename_column :sensor_types, :type, :type_name
  end
end
