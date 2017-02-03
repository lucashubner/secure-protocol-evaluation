class CreateStatistics < ActiveRecord::Migration[5.0]
  def change
    create_table :statistics do |t|
      t.integer :value
      t.references :protocol_type, foreign_key: true

      t.timestamps
    end
  end
end
