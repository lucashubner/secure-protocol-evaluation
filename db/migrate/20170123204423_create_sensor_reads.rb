class CreateSensorReads < ActiveRecord::Migration[5.0]
  def change
    create_table :sensor_reads do |t|
      t.float :value
      t.references :sensor_type, foreign_key: true

      t.timestamps
    end
  end
end
