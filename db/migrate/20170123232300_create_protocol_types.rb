class CreateProtocolTypes < ActiveRecord::Migration[5.0]
  def change
    create_table :protocol_types do |t|
      t.string :protocolName

      t.timestamps
    end
  end
end
